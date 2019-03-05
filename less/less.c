#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <locale.h>

#define version L"version 1.2.1 beta\n"
#define help L"Программа выводит содержимое файла в терминал.Возможны следующие команды:\n/подстрока_для_поиска - поиск в тексте вниз до ближайшей находки.\nsubst /строка_образец/на_что заменять/ - поиск с заменой в тексте.\nчисло - переместить курсор на позицию задаваемую числом.\nwrite ""имя файла"" - сохраняет текст в файл с указанным именем.\nДо имени файла можно указать следующие параметры:\n-n - печать номеров строк перед текстом.\n-v - текущая версия программы.\n-h - справка.\nВ случае указания -v или -h отображения файла не происходит.\n"

#define start() wprintf(L"\E[H")
#define clrscr() wprintf(L"\E[2J")
#define move(y,x) wprintf(L"\E[%d;%dH", y, x)

typedef struct _list_node
{
	wchar_t *str;
	struct _list_node *prev;
	struct _list_node *next;
} list_node;
typedef struct _biderect_list
{
	int maxlength;
	int num_elements;
	list_node *head;
	list_node *tail;
} biderect_list;

int row,col,xg,yg,number,x,y,xg1,yg1,indent,xs;
wchar_t *string;

int wcsatoi(wchar_t *str)
{
	int a=0,b=0,i;
	for (i=0;i<=wcslen(str)-1;i++)
	{
		b=(str[i]-'0');
		a=a*10+b;
	}
	return a;
}

wchar_t* liststr(biderect_list *list)
{
	list_node *tmp = list->head;
	wchar_t *err=NULL;
	int i=-1;
	string=(wchar_t *)malloc(sizeof(wchar_t));
	if (string==NULL)
		return NULL;
	string[0]=L'\0';
	while(tmp)
	{
		string=(wchar_t *)realloc(string,(sizeof(wchar_t)*(wcslen(string)+wcslen(tmp->str)+2)));
		if(string==NULL)
		{
			if(err)
				free(err);
			return NULL;
		}
		err=string;
		wcscat(string,tmp->str);
		i=i+wcslen(tmp->str)+1;
		string[i]=L'\n';
		string[i+1]=L'\0';
		tmp=tmp->next;
	}
	string[i]=L'\0';
	return string;
}

void ps (wchar_t *str)
{
	int i=0;
	for (i=0;i<=wcslen(str)-1;i++)
	{
		if (str[i]==L'\n')
			wprintf(L"\\n");
		else
			wprintf(L"%lc",str[i]);
	}
}

wchar_t* change(wchar_t *str,wchar_t *s1,wchar_t *s2)
{
	int j,start,finish,k=0;
	wchar_t *s=NULL,*sbuf=NULL;
	string=(wchar_t *)malloc(sizeof(wchar_t)*(wcslen(str)+1));
	if (!string)
		return NULL;
	wcscpy(string,str);
	while ((s=wcsstr(((string)+k),s1))!=NULL)
		if (s)
		{
			start=s-(string);
			finish=s-(string)+wcslen(s1)-1;
			sbuf=(wchar_t *)malloc(sizeof(wchar_t)*(wcslen(string)+wcslen(s2)-wcslen(s1)+1));
			if (!sbuf)
			{
				free(string);
				return NULL;
			}
			for(j=0;j<=start-1;j++)
				sbuf[j]=(string[j]);
			for(j=start;j<=start+wcslen(s2)-1;j++)
				sbuf[j]=s2[j-start];
			for(j=finish+1;j<=wcslen(string)-1;j++)
				sbuf[start+wcslen(s2)-1+j-finish]=(string[j]);
			sbuf[wcslen(string)-wcslen(s1)+wcslen(s2)]=L'\0';
			free(string);
			(string)=NULL;
			(string)=(wchar_t *)malloc(sizeof(wchar_t)*(wcslen(sbuf)+1));
			if(!string)
			{
				free(sbuf);
				return NULL;
			}
			wcscpy((string),sbuf);
			free(sbuf);
			sbuf=NULL;
			if (wcscmp(s1,L"\n\n"))
				k=start+wcslen(s2);
		}
	return string;
}
		
int search(biderect_list *list,wchar_t *str)
{
	int i=1;
	wchar_t *s;
	xs=xg1;
	list_node *tmp=list->head;
	while(tmp)
	{
		if (i>=yg1)
		{
			s=wcsstr(tmp->str+xs-indent,str);
			if (s)
			{
				yg1=i;
				yg=i;
				xg1=(s-(tmp->str))+1+indent;
				/*if ((xg1+col)<=list->num_elements)
				{
					x=indent+1;
					xg=xg1+col;
				}
				else
				{
					xg=list->num_elements;
					x=xg1-col*(xg1/col);
				}*/
				if (xg1>col)
					x=col;
				else
					x=xg1;
				if (yg1>row-1)
					y=row-1;
				else
					y=yg1;
				xg=xg1;
				return 1;
			}
			xs=indent;
		}
		tmp=tmp->next;
		i++;
	}
	tmp=list->head;
	xs=indent;
	i=1;
	while(tmp)
	{
		s=wcsstr(tmp->str+xs-indent,str);
		if (s)
		{
			yg1=i;
			yg=i;
			xg1=(s-(tmp->str))+1+indent;
			/*if ((xg1+col)<=list->num_elements)
			{
				x=indent+1;
				xg=xg1+col;
			}
			else
			{
				xg=list->num_elements;
				x=xg1-col*(xg1/col);
			}*/
			if (xg1>col)
				x=col;
			else
				x=xg1;
			if (yg1>row-1)
				y=row-1;
			else
				y=yg1;
			xg=xg1;
			return 1;
		}
		tmp=tmp->next;
		i++;
		if (i>yg1)
			break;
	}
	return 0;
}

int command(wchar_t *str)
{
	int i=1;
	wchar_t *s,wc;
	s=(wchar_t *)malloc(sizeof(wchar_t)*7);
	if (s==NULL)
		return -1;
	if (str==L"write")
	{
		s[0]=L'w';
	}
	else
		s[0]=L's';
	while(1)
	{
		wc=getwc(stdin);
		if (wc==L'\n')
		{
			free(s);
			return 1;
		}
		if (wc==L' ')
		{
			wprintf(L" ");
			break;
		}
		wprintf(L"%lc",wc);
		s[i]=wc;
		i++;
		if (i==6)
			break;
	}
	s[i]=L'\0';
	if (wcscmp(s,str)==0)
	{
		free(s);
		return 0;
	}
	else
	{
		free(s);
		return 1;
	}
}

int digits(int a)
{
	int n=0;
	unsigned int b=a;
	do
	{
		b=b/10;
		n++;
	}
	while(b!=0);
	return n;
}

void printnumber(int x,int k,int n)
{
	int i,dif;
	dif=n-k;
	wprintf(L"%d",x);
	for(i=1;i<=dif;i++)
		wprintf(L" ");
	wprintf(L":");
}
	

void emptystr(void)
{
	int i;
	for(i=1;i<=(col-1);i++)
		wprintf(L" ");
}

int length(wchar_t *str)
{
	int i=0;
	while (str[i]!=L'\0')
		i++;
	return i;
}

void printstr(wchar_t *str)
{
	int k=0;
	if (xg>col)
		k=xg-col;
	if (k<=length(str))
	while (str[k]!=L'\0')
	{
		wprintf(L"%lc",str[k]);
		k++;
		if (xg>col)
		{
			if ((k-(xg-col))>(col-indent-1)) break;
		}
		else
			if (k>col-indent-1) break;
	}
	wprintf(L"\n");
}

biderect_list* creatlist(void)
{
	biderect_list *tmp = (biderect_list*)malloc(sizeof(biderect_list));
	tmp->num_elements = 0;
	tmp->maxlength = 0;
	tmp->head = tmp->tail = NULL;
	return tmp;
}

void deletelist(biderect_list **list) 
{
	list_node *tmp = (*list)->head;
	list_node *next = NULL;
		while (tmp) 
		{
			next = tmp->next;
			free(tmp->str);
			free(tmp);
			tmp = next;
		}
	free(*list);
	(*list) = NULL;
}

int push(biderect_list *list, wchar_t *str) 
{
	list_node *tmp = (list_node*)malloc(sizeof(list_node));
	if (tmp == NULL) 
		return -1;
	tmp->str = str;
	tmp->next = NULL;
	tmp->prev = list->tail;
	if (list->tail) 
		list->tail->next = tmp;
	list->tail = tmp;
	if (list->head == NULL) 
		list->head = tmp;
//	if (list->num_elements!=1)
		list->num_elements++;
	if (wcslen(str)>list->maxlength) 
		list->maxlength=length(str);
	if (1>list->maxlength)
		list->maxlength=1;
	return 0;
}

int changeemptystring(biderect_list *list,wchar_t *s1)
{
	list_node *tmp = list->head;
	int flag = 0;
	while(tmp)
	{
		if/*(wcslen(tmp->str)==0)*/(tmp->str[0]==L'\0')
		{
			if (wcslen(s1)==0)
			{
				if (tmp->prev) tmp->prev->next = tmp->next;
				if (tmp->next) tmp->next->prev = tmp->prev;
				if (!tmp->prev) list->head = tmp->next;
				if (!tmp->next) list->tail = tmp->prev;
				if (tmp->str)
					free(tmp->str);
				if (tmp)
					free(tmp);
				list->num_elements--;
				tmp=list->head;
				flag = 1;
			}
			else
			{
				free(tmp->str);
				tmp->str=(wchar_t *)malloc(sizeof(wchar_t)*(wcslen(s1)));
				wcscpy(tmp->str,s1);
			}
		}
		if (!flag)
			tmp=tmp->next;
		flag=0;
	}
	if (list->num_elements==0)
	{
		string=(wchar_t *)malloc(sizeof(wchar_t));
		string[0]=L'\0';
		push(list,string);
	}
	return 0;
}

int strlist(biderect_list *list,wchar_t *s)
{
	wchar_t *str=NULL,c,*err=NULL;
	int i=0,j=0;
	while(1)
	{
		c=s[i];
		if (c==L'\0')
		{
			str=(wchar_t *)realloc(str,(sizeof(wchar_t)*(j+1)));
			if(str==NULL)
			{
				if (err)
					free(err);
				return 0;
			}
			str[j]=L'\0';
			push(list,str);
			str=err=NULL;
			break;
		}
		j++;
		if(c==L'\n')
		{
			str=(wchar_t *)realloc(str,(sizeof(wchar_t)*j));
			if(str==NULL)
			{
				if (err)
					free(err);
				return 0;
			}
			str[j-1]=L'\0';
			if(push(list,str)==-1)
			{
				if(str)
					free(str);
				return -1;
			}
			j=0;
			str=err=NULL;
			i++;
			continue;
		}
		str=(wchar_t *)realloc(str,(sizeof(wchar_t)*j));
		if (str==NULL)
		{
			if (err)
				free(err);
			return 0;
		}
		err=str;
		str[j-1]=c;
		i++;
	}
	return 1;
}

int readf(FILE* f,biderect_list *list)
{
	wint_t wi;
	wchar_t *str = NULL,*err = NULL,wc;
	int count=0;
	while(1)
	{
		wc = fgetwc(f);
		if(feof(f))
			break;
		err = (wchar_t*)realloc(err,(sizeof(wchar_t)*(count+1)));
		if(err == NULL)
		{
			free(str);
			return -1;
		}
		err[count]=wc;
		str=err;
		count++;
		if(wc=='\n')
		{
			str[count-1]=L'\0';
			if(push(list,str)==-1) 
				return -1;
			str=NULL;
			err=NULL;
			count=0;
		}
	}
	return 0;
}

void printlist(biderect_list *list)
{
	list_node *tmp = list->head;
	int i=1,lim=1,n,k;
	n=digits(list->num_elements);
	if (yg>(row-1))
		lim=yg-(row-1)+1;
	while (tmp)
	{
		if (i==lim+row-1) break;
		if (i>=lim)
		{
			if (number)
			{
				k=digits(i);
				printnumber(i,k,n);
			}
			if (xg>col)
				wprintf(L"<");
			else
				wprintf(L"|");
			printstr(tmp->str);
		}
		tmp = tmp->next;
		i++;
	}
}

void upgradelist(biderect_list *list)
{
	start();
	clrscr();
	printlist(list);
	move(row,1);
	emptystr();
	move(row,1);
	wprintf(L"%d(%d)",yg1,list->num_elements);
}

void printstr1(wchar_t *str)
{
	int i=0;
	while(str[i]!=L'\0')
	{
		wprintf(L"%lc",str[i]);
		i++;
	}
}

void printlist1(biderect_list *list)
{
	list_node *tmp=list->head;
	while(tmp)
	{
		printstr(tmp->str);
		wprintf(L"\n");
		tmp = tmp->next;
	}
}

int savelist(FILE *f,biderect_list *list)
{
	wchar_t c=L'\n';
	list_node *tmp = list->head;
	while (tmp)
	{
		if(fwprintf(f,L"%ls\n",tmp->str) < (wcslen(tmp->str)+1))
			return -1;
		tmp = tmp->next;
	}
	return 0;
}

int main(int argc,char *argv[])
{
	FILE *f;
	struct termios savetty;
	struct termios tty;
	struct winsize w;
	char *file=NULL;
	wchar_t wc,*num=NULL,*qwerty = NULL;
	biderect_list *list;
	char *opts = "+nvh";
	wchar_t v = 0,h = 0,def = 0,*lastsearch=NULL;
	int opt,q=0;
	extern int optind;
	
	setlocale(LC_ALL, "");

	if (isatty(0))
	{
		if (argc==1)
		{
			wprintf(L"Введите имя файла или параметры\n");
			return 1;
		}
	
		number=0;
		while ((opt = getopt(argc,argv,opts))!=-1)
		{
			switch(opt)
			{
				case 'n':number = 1; break;
				case 'v': v = 1;break;
				case 'h': h = 1;break;
			}
		}

		if (optind>4)
		{
			wprintf(L"Введите до трех параметров и имя файла\n");
			return 1;
		}

		if (argc>(optind+1)) wprintf(L"Параметры указанные после имени файла будут проигнорированы\n");

		if ((v)||(h))
		{
			if (v) wprintf(L"%ls",version);
			if (h) wprintf(L"%ls",help);
			return 0;
		}

		if((f = fopen(argv[optind],"r"))==NULL)
		{
			wprintf(L"Не удаётся открыть файл %ls\n",argv[optind]);
			return 2;
		}
	
		list=creatlist();
		if (list==NULL)
		{
			wprintf(L"Ошибка выделения памяти\n");
			return 3;
		}
		if (readf(f,list)==-1)
		{
			deletelist(&list);
			wprintf(L"Ошибка выделения памяти\n");
			return 3;
		}
	
		fclose(f);
	}
	else
	{
		list = creatlist();
		if (list==NULL)
		{
			wprintf(L"Ошибка выделения памяти\n");
			return 3;
		}
		if (readf(stdin,list)==-1)
		{
			deletelist(&list);
			wprintf(L"Ошибка выделения памяти\n");
			return 3;
		}
	}

	if(!isatty(1))
	{
		printlist1(list);
		deletelist(&list);
		return 0;
	}
	
	/*if (list->maxlength==0)
	{
		list->maxlength=1;
	}*/

	ioctl(0, TIOCGWINSZ, &w);
	row = w.ws_row;
	col = w.ws_col;
	tcgetattr (0, &tty);
	savetty = tty;
	tty.c_lflag &= ~(ICANON|ECHO|ISIG);
	tty.c_cc[VMIN] = 1;
	tcsetattr (0, TCSAFLUSH, &tty);
	
	qwerty = (wchar_t *)malloc(sizeof(wchar_t));
	qwerty[0] = L'\0';	
	if (list->num_elements == 0)
		push(list,qwerty);
	start();
	clrscr();
	if (number)
		indent=digits(list->num_elements)+2;
	else
		indent=1;
	x=xg=xg1=indent+1;
	y=yg=yg1=1;
	printlist(list);
	move(row,1);
	wprintf(L"%d(%d)",1,list->num_elements);
	move(y,x);
	for(;;)
	{
		wc=getwc(stdin);
		if (wc==L'[')
		{
			switch(getwc(stdin))
			{
				case L'A': /*Вверх*/
				{
					if (yg1==1) 
						break;
					y--;
					yg1--;
					if (y==0)
					{
						yg--;
						y=1;
						upgradelist(list);
					}
					else
					{
						if (yg<(row-1))
							yg--;
						upgradelist(list);
					}
					move(y,x);
					break;
				}
				case L'B': /*Вниз*/
				{
					if (yg1==list->num_elements)
						break;
					/*{
						if (list->num_elements<=(row-1))
							break;
						if (y==row-1)
							break;
					}*/
					y++;
					yg1++;
					if (yg<row-1)
						yg++;
					if (y==row)
					{
						yg++;
						y=row-1;
						upgradelist(list);
					}
					else
					{
						upgradelist(list);
					}
					move(y,x);
					break;
				}
				case L'C': /*Вправо*/
				{
				/*	if (xg1==list->maxlength)
					{
						if (list->maxlength<=(col-1))
							break;
						if (x==col)
							break;
					}*/
					if (xg1==list->maxlength+indent)
						break;
					x++;
					xg1++;
					if (xg<col)
						xg++;
					if(x>col)
					{
						xg++;
						x=col;
						upgradelist(list);
					}
					move(y,x);
					break;
				}
				case L'D': /*Влево*/
				{
					if(xg1==(indent+1)) 
						break;
					x--;
					xg1--;
					if (xg<col)
						xg--;
					if(x==indent)
					{	xg--;
						x=indent+1;
						upgradelist(list);
					}
					move(y,x);
					break;
				}
			}
		}
		if ((wc==L'q')||(wc==L'Q')) break;
		if (wc==L'w')
		{
			int flag;
			wchar_t c;
			char *err;
			move(row,1);
			emptystr();
			move(row,1);
			wprintf(L"w");
			flag=command(L"write");
			if (flag==-1)
			{
				move(row,1);
				emptystr();
				wprintf(L"ошибка");
				move(y,x);
			}
			else
			{
				if (flag==1)
				{
					move(row,1);
					emptystr();
					move(row,1);
					wprintf (L"такой команды нет");
					move(y,x);
				}
				else
				{
					if (getwc(stdin)!='\"')
					{
						move(row,1);
						emptystr();
						move(row,1);
						wprintf(L"такой команды нет");
						move(y,x);
					}
					else
					{
						int i=0;
						wprintf(L"\"");
						while(1)
						{
							c=getwc(stdin);
							if (c==L'\n')
							{
								move(row,1);
								emptystr();
								move(row,1);
								wprintf (L"некорректно введено имя файла");
								move(y,x);
								if (file)
									free(file);
								break;
							}
							if (c==L'\"')
							{
								wprintf(L"\"");
								file=(char *)realloc(file,sizeof(char)*(i+1));
								if(!file)
								{
									if(err)
										free(err);
									move(row,1);
									emptystr();
									move(row,1);
									wprintf(L"ошибка выделения памяти");
									move(y,x);
									break;
								}
								err=file;
								file[i]='\0';
								f=fopen(file,"w");
								if (f!=NULL)
								{
									if (savelist(f,list)==0)
									{
										fclose(f);
										move(row,1);
										emptystr();
										move(row,1);
										wprintf(L"Запись произведена успешно");
										move(y,x);
									}
									else
									{
										fclose(f);
										move(row,1);
										emptystr();
										move(row,1);
										wprintf(L"Файл записан с ошибкой");
										move(y,x);
									}
								}
								else
								{
									move(row,1);
									emptystr();
									move(row,1);
									wprintf(L"не удаётся открыть файл");
									move(y,x);
								}
								free(file);
								break;
							}
							if (c==L'\\')
							{
								wprintf(L"\\");
								c=getwc(stdin);
								if (c==L'"')
									c=L'"';
								if (c==L'\\')
									c=L'\\';
								if (c==L'\n')
								{
									move(row,1);
									emptystr();
									move(row,1);
									wprintf (L"некорректно введено имя файла");
									move(y,x);
									if (file)
										free(file);
									break;
								}
							}
							file=(char *)realloc(file,(sizeof(char)*(i+1)));
							if(!file)
							{
								if(err)
									free(err);
								move(row,1);
								emptystr();
								move(row,1);
								wprintf(L"ошибка выделения памяти");
								move(y,x);
								break;
							}
							err=file;
							file[i]=(char)c;
							wprintf(L"%lc",c);
							i++;
						}
					}
				}
			}
		}
		if ((wc>=L'0')&&(wc<=L'9'))
		{
			int i=1;
			num=(wchar_t *)malloc(64*sizeof(wchar_t));
			if (num==NULL)
			{
				move(row,1);
				emptystr();
				move(row,1);
				wprintf(L"Ошибка выделения памяти");
				move(y,x);
			}
			num[0]=wc;
			move(row,1);
			emptystr();
			move(row,1);
			wprintf(L"%lc",wc);
			while (1)
			{
				wc=getwc(stdin);
				if ((wc>=L'0')&&(wc<=L'9'))
				{
					wprintf(L"%lc",wc);
					num[i]=wc;
				}
				else
				{
					if(wc=='\n')
					{
						num[i]=L'\0';
						i=wcsatoi(num);
						if (i>list->num_elements)
						{
							move(row,1);
							emptystr();
							move(row,1);
							wprintf(L"в файле %d строк",list->num_elements);
							move(y,x);
							break;
						}
						if (i==0)
						{
							move(row,1);
							emptystr();
							move(row,1);
							wprintf(L"отсчет начинается с 1");
							move(y,x);
							break;
						}
						yg=i;
						yg1=i;
						if(i>=row) 
							y=row-1;
						else
							y=i;
						upgradelist(list);
						break;
					}
					else
					{
						move(row,1);
						emptystr();
						move(row,1);
						wprintf(L"некорректный ввод");
						move(y,x);
						break;
					}
				}
				i++;
			}
			free(num);
			move(y,x);
		}
		if (wc==L'/')
		{
			wchar_t c,*s=NULL,*err=NULL;
			int i=0;
			move(row,1);
			emptystr();
			move(row,1);
			wprintf(L"/");
			while(1)
			{
				c=getwc(stdin);
				if (c==L'\n')
				{
					if (s==NULL)
					{
						if (lastsearch)
						{
							s=(wchar_t *)malloc(sizeof(wchar_t)*(wcslen(lastsearch)+1));
							wcscpy(s,lastsearch);
						}
						else
						{
							move(row,1);
							emptystr();
							move(row,1);
							wprintf(L"поиска еще производилось");
							move(y,x);
							break;
						}
					}
					else
						s[i]=L'\0';
					if (search(list,s))
					{
						upgradelist(list);
						move(y,x);
					}
					else
					{
						move(row,1);
						emptystr();
						move(row,1);
						wprintf(L"не найдено");
						move(y,x);
					}
					break;
				}
				s=(wchar_t *)realloc(s,(sizeof(wchar_t)*(i+2)));
				if(!s)
				{
					if(err)
						free(err);
					move(row,1);
					emptystr();
					move(row,1);
					wprintf(L"ошибка выделения памяти");
					move(y,x);
				}
				err=s;
				s[i]=c;
				i++;
				wprintf(L"%lc",c);
			}
			if(s)
			{
				if(lastsearch)
					free(lastsearch);
				lastsearch=(wchar_t *)malloc(sizeof(wchar_t)*(wcslen(s)+1));
				if(!lastsearch)
				{
					move(row,1);
					emptystr();
					move(row,1);
					wprintf(L"ошибка выделения памяти последний поиск утерян");
					move(y,x);
				}
				else
				{
					wcscpy(lastsearch,s);
					free(s);
				}
			}
		}
		if (wc==L's')
		{
			int flag,i=0;
			wchar_t c,*sample=NULL,*substitution=NULL,*lstr=NULL,*newlstr=NULL,*err=NULL;
			move(row,1);
			emptystr();
			move(row,1);
			wprintf(L"s");
			flag=command(L"subst");
			if (flag==-1)
			{
				move(row,1);
				emptystr();
				wprintf(L"ошибка");
				move(y,x);
			}
			else
			{
				if (flag==1)
				{
					move(row,1);
					emptystr();
					move(row,1);
					wprintf (L"такой команды нет");
					move(y,x);
				}
				else
				{
					{
						if (getwc(stdin)!='/')
						{
							move(row,1);
							emptystr();
							move(row,1);
							wprintf(L"неверный формат команды");
							move(y,x);
						}
						else
						{
							wprintf(L"/");
							while(1)
							{
								c=getwc(stdin);
								if (c==L'/')
								{
									wprintf(L"/");
									if(sample)
										sample[i]='\0';
									break;
								}
								if (c==L'\\')
								{
									wprintf(L"\\");
									c = getwc(stdin);
									if (c==L'/')
										c=L'/';
									else
									{
									if (c==L'n')
									{
										wprintf(L"n");
										c=L'\n';
										sample=(wchar_t *)realloc(sample,(sizeof(wchar_t)*(i+2)));
										if(!sample)
										{
											if(err)
												free(err);
											i=-1;
											break;
										}
										err=sample;
										sample[i]=c;
										i++;
										continue;
									}
									else
									{
									if (c==L'\\')
									{
										wprintf(L"\\");
										c=L'\\';
										sample=(wchar_t *)realloc(sample,(sizeof(wchar_t)*(i+2)));
										if(!sample)
										{
											if(err)
												free(err);
											i=-1;
											break;
										}
										err=sample;
										sample[i]=c;
										i++;
										continue;
									}
									else
									{
										sample=(wchar_t *)realloc(sample,(sizeof(wchar_t)*(i+2)));
										if(!sample)
										{
											if(err)
												free(err);
											i=-1;
											break;
										}
										err=sample;
										sample[i]=L'\\';
										i++;
									}
									}
									}
								}
								sample=(wchar_t *)realloc(sample,(sizeof(wchar_t)*(i+2)));
								if(!sample)
								{
									if(err)
										free(err);
									i=-1;
									break;
								}
								err=sample;
								sample[i]=c;
								i++;
								wprintf(L"%lc",c);
							}
							if(i!=-1)
							{
							i=0;
							if(sample==NULL)
							{
								move(row,1);
								emptystr();
								move(row,1);
								wprintf(L"неверный формат команды");
							}
							else
							{
								while(1)
								{
									c=getwc(stdin);
								//	if (c==27) {getwc(stdin);getwc(stdin);continue;}
									if (c==L'/')
									{
										if(substitution)
											substitution[i]=L'\0';
										else
										{
											substitution=(wchar_t *)malloc(sizeof(wchar_t));
											substitution[0]=L'\0';
										}

										if (!wcscmp(sample,L"\n"))
										{
											changeemptystring(list,substitution);
											if (yg1>list->num_elements)
											{
												yg1=list->num_elements;
												yg=list->num_elements;
												if (yg1>(row-1))
													y=row-1;
												else
													y=yg1;
											}
											if(number)
											{
											xg=xg+indent-digits(list->num_elements)-2;
											xg1=xg1+indent-digits(list->num_elements)-2;
											indent=digits(list->num_elements)+2;
											}
											upgradelist(list);
											move(y,x);
											break;
										}

										lstr=liststr(list);
										if(!lstr)
										{
											move(row,1);
											emptystr();
											move(row,1);
											wprintf(L"ошибка выделения памяти");
											move(y,x);
											break;
										}
										deletelist(&list);
										list=creatlist();
										/*if (!wcscmp(sample,L"\n"))
										{
											sample=(wchar_t *)realloc(sample,(sizeof(wchar_t)*3));
											sample[0]=sample[1]=L'\n';
											sample[2]=L'\0';
										}*/
										newlstr=change(lstr,sample,substitution);
										if(!newlstr)
										{
											free(lstr);
											move(row,1);
											emptystr();
											move(row,1);
											wprintf(L"ошибка выделения памяти");
											move(y,x);
											break;
										}
										flag=strlist(list,newlstr);
										if ((flag==-1)||(flag==0))
										{
											upgradelist(list);
											move(row,1);
											emptystr();
											move(row,1);
											wprintf(L"ошибка выделения памяти");
											move(y,x);
											free(lstr);
											free(newlstr);
											break;
										}
										if (yg1>list->num_elements)
										{
											yg1=list->num_elements;
											yg=list->num_elements;
											if (yg1>(row-1))
												y=row-1;
											else
												y=yg1;
										}
										if (number)
										{
											xg=xg+indent-digits(list->num_elements)-2;
											xg1=xg1+indent-digits(list->num_elements)-2;
											indent=digits(list->num_elements)+2;
											if (x<indent+1) x=indent+1;
										}
										upgradelist(list);
										free(lstr);
										free(newlstr);
										move(y,x);
									//	upgradelist(list);
									//	move(y,x);
										break;
									}
									if (c==L'\\')
									{
										wprintf(L"\\");
										c = getwc(stdin);
										if (c==L'/')
											c=L'/';
										else
										{
										if (c==L'n')
										{
											wprintf(L"n");
											c=L'\n';
											substitution=(wchar_t *)realloc(substitution,(sizeof(wchar_t)*(i+2)));
											if(!substitution)
											{
												if(err)
													free(err);
												i=-1;
												break;
											}
											err=substitution;
											substitution[i]=c;
											i++;
											continue;
										}
										else
										{
										if (c==L'\\')
										{
											wprintf(L"\\");
											c=L'\\';
											substitution=(wchar_t *)realloc(substitution,(sizeof(wchar_t)*(i+2)));
											if(!substitution)
											{
												if(err)
													free(err);
												i=-1;
												break;
											}
											err=substitution;
											substitution[i]=c;
											i++;
											continue;
										}
										else
										{
											substitution=(wchar_t *)realloc(substitution,(sizeof(wchar_t)*(i+2)));
											if(!substitution)
											{
												if(err)
													free(err);
												i=-1;
												break;
											}
											err=substitution;
											substitution[i]=L'\\';
											i++;
										}
										}
										}
									}
									substitution=(wchar_t *)realloc(substitution,(sizeof(wchar_t)*(i+2)));
									if(!substitution)
									{
										if(err)
											free(err);
										i=-1;
									}
									err=substitution;
									substitution[i]=c;
									wprintf(L"%lc",c);
									i++;
								}
								if(i==-1)
								{
									move(row,1);
									emptystr();
									move(row,1);
									wprintf(L"ошибка выделения памяти");
									move(y,x);
								}
								if(substitution)
									free(substitution);
								free(sample);
							}
							}
							else
							{
								move(row,1);
								emptystr();
								move(row,1);
								wprintf(L"ошибка выделения памяти");
								move(y,x);
							}
						}
					}
				}
			}
		}

		ioctl(0, TIOCGWINSZ, &w);
		row = w.ws_row;
		col = w.ws_col;

	}
	if(lastsearch)
		free(lastsearch);
	tcsetattr(0,TCSAFLUSH,&savetty);	
	deletelist(&list);
	start();
	clrscr();
	return 0;
}
