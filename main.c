#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdint-gcc.h>
#define _GNU_SOURCE
#include <string.h>
#include <process.h>
#include "utils.h"
#include <ctype.h>
//SYNOPSIS
//replace OPT <from> <to> -- <filename> [<filename>]*
//this function check if the command line is written in the right form
/*
@Param
@cmd is command written by user like replace -i b bitel paysky -- bitel.txt paysky.txt.
@FrmWord the word that need to change in our case it will be bitel.
@ToWord the word the replace FrmWord in our case it will be paysky
@fileName the name of file the we will execute our cmd on it (bitel.txt)--(paysky.txt)
@opt the option available in cmd

-b: if specified, the replace utility creates a backup copy of each file on
which a replace operation is performed before modifying it.
-f: if specified, the replace utility only replaces the first occurrence of string
from in each file.
-l: if specified, the replace utility only replaces the last occurrence of string from in each file.
-i: if specified, the replace utility performs the search for string from in a
case insensitive way.
if this function execute successfully it return RET_OK (0)
else it return RET_KO
*/
int8_t Parsing_command(uint8_t * cmd,uint8_t*FrmWord,uint8_t*ToWord,uint8_t ***fileName, int16_t *No_Files,uint8_t*opt)
{
	uint8_t * ptr=NULL;
	uint8_t * Tptr=NULL;
	uint8_t * sptr=NULL;
	uint8_t * Fptr=NULL;
	uint8_t * cptr=NULL;
	uint8_t   OPT=0;
	uint16_t  j= 0;
	uint16_t  j2=0;
	int8_t iRet=RET_OK;
	uint8_t LastOPT=0;
	uint8_t OPT_b_count=0;
	uint8_t OPT_f_count=0;
	uint8_t OPT_l_count=0;
	uint8_t OPT_i_count=0;
	uint8_t flag=0;
	uint8_t countFiles=0;
	if(StrnCmp(cmd,"replace",strlen("replace"))==0)
	{
		//check number of OPT in cmd
		Tptr = &cmd[strlen("replace")+1];
		ptr  = StrStr(cmd," -- ");
		if(ptr)
        {
		Fptr=ptr;
		cptr=ptr;
		//check if name is written right or not
		cptr+=1;
		cptr=StrStr(cptr," -- ");
        }
		if(cptr)
        {
           iRet=RET_KO;
        }
		if(ptr&&iRet==RET_OK)
		{
			while(&Tptr[j]!= ptr)
			{
				if(Tptr[j] == '-')
				{
					switch(Tptr[j+1])
					{
					case 'b':
					    if(Tptr[j+2]==' ')
                        {
                        OPT|= b;
						OPT_b_count++;
						LastOPT=b;
                        }
						break;
					case 'f':
					    if(Tptr[j+2]==' ')
                        {
						OPT|=f;
						OPT_f_count++;
						LastOPT=f;
                        }
						break;
					case 'l':
					    if(Tptr[j+2]==' ')
                        {
						OPT|=l;
						LastOPT=l;
						OPT_l_count++;
                        }
						break;
					case 'i':
					    if(Tptr[j+2]==' ')
                        {
						OPT|=i;
						LastOPT=i;
						OPT_i_count++;
                        }
						break;
					default :
						//iRet=RET_KO;
						break;

					}
				}
				if(j>strlen((const char*)cmd))
                {

                 iRet=RET_KO;
                 break;
                }
				j++;
			}

			if((OPT==0)
					||(OPT_b_count>=2)
					||(OPT_f_count>=2)
					||(OPT_i_count>=2)
					||(OPT_l_count>=2))
			{
				iRet=RET_KO;
			}

		}
		else
		{
			iRet=RET_KO;
		}

	}
	else
	{

		iRet= RET_KO;
	}
	//extract the words that we want to change
	if(iRet==RET_OK)
	{
		switch(LastOPT)
		{
		case b:
			sptr  = StrStr(cmd,"-b ");
			break;
		case f:
			sptr  = StrStr(cmd,"-f ");
			break;
		case l:
			sptr  = StrStr(cmd,"-l ");
			break;
		case i:
			sptr  = StrStr(cmd,"-i ");
			break;
		}
		if(sptr)
		{
			sptr+=3;
			if(sptr!=ptr)
			{
				j=0;
				while(&sptr[j]!= ptr)
				{
					if(flag==0)
					{
						if(sptr[j]!=' ')
						{
							FrmWord[j]=sptr[j];
							j++;
						}
						else
						{
							flag=1;
							sptr=&sptr[j]+sizeof(uint8_t);
							j=0;
						}
					}
					else
					{
						if(&sptr[j]== ptr)
						{
							iRet=RET_KO;
						}
						else
						{
							ToWord[j]=sptr[j];
							j++;
						}

					}

				}
			}
			else
			{
				iRet=RET_KO;
			}

		}
		if(!(strlen((const char *)FrmWord)&&(strlen((const char *)ToWord))))
        {
            iRet=RET_KO;
        }

	}
	if(iRet==RET_OK)
    {
        while(Fptr)
        {
            Fptr=StrStr(Fptr,".txt");
            if(Fptr)
            {
                  Fptr++;
                  countFiles++;
            }
            if(countFiles==0)
            {
                iRet=RET_KO;
            }
        }

        //allocate memory for file Name
        //The max uint8_t in file name is 30 uint8_t ex.MAIN.txt must be less than 30
    if(iRet==RET_OK)
    {
        *fileName=(uint8_t**)calloc(countFiles+1,sizeof(uint8_t*));
        *No_Files=countFiles;
        *opt=OPT;
        for(j=0;j<countFiles+1;j++)
        {
         (*fileName)[j]=(uint8_t*)calloc(30,sizeof(uint8_t));
        }
        Fptr=ptr;
        Fptr+=4;
        for(j=0;j<countFiles;)
        {
           if(Fptr[j2]!=0)
           {
                if(Fptr[j2]!=' ' )
                {
                  (*fileName)[j][j2]  =Fptr[j2];
                  j2++;
                }
                else
                {
                    (*fileName)[j][j2]  =0;
                    Fptr=&Fptr[j2]+1;
                    j++;
                    j2=0;

                }
           }
           else
           {
               break;
           }

        }
        if(j<countFiles)
        {
            iRet=RET_KO;
        }
         else
         {
               (*fileName)[j]=0;
         }
    }

}

	return iRet;
}
/*
@Filedirection the path of file that will read from it ex("D:\\bitel.txt")
@data carry the content of file.
@return
if this function execute successfully it return RET_OK (0)
else it return RET_KO
*/

int8_t ReadFile(uint8_t* Filedirection,uint8_t* data)
{
        int8_t iRet=RET_OK;
        FILE* Fhandle=fopen((char*)Filedirection,"r");
        uint32_t filesize=0;
        if(Fhandle)
        {
        fseek(Fhandle, 0, SEEK_END);
        filesize = ftell(Fhandle);
        fseek(Fhandle, 0, SEEK_SET);
        fread(data,filesize,1,Fhandle);
        fclose(Fhandle);
        }
        else
        {
            iRet=RET_KO;
        }
        return iRet;
}
/*
@Filedirection the path of file that will read from it ex("D:\\bitel.txt")
@data  date that will written on the  file.
@return
if this function execute successfully it return RET_OK (0)
else it return RET_KO
*/
int8_t WriteFile(uint8_t* Filedirection,uint8_t* data)
{
        int8_t iRet=RET_OK;
        remove((const char*)Filedirection);
        FILE* Fhandle=fopen((const char*)Filedirection,"w");
        if(Fhandle)
        {
        fseek(Fhandle, 0, SEEK_SET);
        fwrite(data,strlen((const char*)data),1,Fhandle);
        fclose(Fhandle);

        }
        else
        {
            iRet=RET_KO;
        }
        return iRet;
}

/*
@filename the path of file that will read from it ex("D:\\bitel.txt")
@data  date that will written on the  file.
@return
if this function execute successfully it return RET_OK (0)
else it return RET_KO
*/
int8_t Wirtebackupfile(uint8_t * filename ,uint8_t* data)
{
        int8_t iRet=RET_OK;
        uint8_t tmp[100]={0};
        StrCpy(tmp,filename);
        strcat((char*)tmp,".bk.txt");
        FILE* Fhandle=fopen((char*)tmp,"r");
        if(Fhandle)
        {
            fclose(Fhandle);
            remove((const char*)tmp);

        }
        Fhandle=fopen((const char*)tmp,"w");
        if(Fhandle)
        {
        fseek(Fhandle, 0, SEEK_SET);
        fwrite(data,strlen((const char*)data),1,Fhandle);
        fclose(Fhandle);

        }
        else
        {
            iRet=RET_KO;
        }
        return iRet;
}

/*
this function search a substring in a big string
@str1 whole word "Pay sky is E_payment company"
@str2 the word need to search in str1 like E_payment
@return
if success
return pointer to the first char
else return 0

*/
uint8_t* stristr( const uint8_t* str1, const uint8_t* str2 )
{
    const uint8_t* p1 = str1 ;
    const uint8_t* p2 = str2 ;
    const uint8_t* r = *p2 == 0 ? str1 : 0 ;

    while( *p1 != 0 && *p2 != 0 )
    {
        if( tolower( (uint8_t)*p1 ) == tolower( (uint8_t)*p2 ) )
        {
            if( r == 0 )
            {
                r = p1 ;
            }

            p2++ ;
        }
        else
        {
            p2 = str2 ;
            if( r != 0 )
            {
                p1 = r + 1 ;
            }

            if( tolower( (uint8_t)*p1 ) == tolower( (uint8_t)*p2 ) )
            {
                r = p1 ;
                p2++ ;
            }
            else
            {
                r = 0 ;
            }
        }

        p1++ ;
    }

    return *p2 == 0 ? (uint8_t*)r : 0 ;
}
/*
this function used to replace word with another word in string
@s the original string bitel is E_payment company
@ oldW the word need to change E_payment
@newW  the word that replace oldW paysky
@result the new word will copy on it so it will be paysky is E_payment company
@OPT option available
b: if specified, the replace utility creates a backup copy of each file on
which a replace operation is performed before modifying it.
-f: if specified, the replace utility only replaces the first occurrence of string
from in each file.
-l: if specified, the replace utility only replaces the last occurrence of string from in each file.
-i: if specified, the replace utility performs the search for string from in a
case insensitive way.
if this function execute successfully it return RET_OK (0)
else it return RET_KO
note that OPT can be 1 or more like (-i-b),(-f -b),(-l -b),(-f -l),(-f-l-b) on;y
and the arrangement of option in cmd is not mandatory
ex replace -b -i bitel paysky -- filename.txt
ex replace -i -b bitel paysky -- filename.txt
the two previous cmd will give the same output
*/
int8_t replaceWord(const uint8_t  *s, const uint8_t  *oldW, const uint8_t *newW,uint8_t **result,uint8_t OPT)
{
    int32_t j, cnt = 0;
    int32_t newWlen = strlen((char*)newW);
    int32_t oldWlen = strlen((char*)oldW);
    int8_t  iRet=RET_OK;
    int32_t cnt2=0;
    for (j = 0; s[j] != '\0'; j++)
    {
        if (StrStr(&s[j], oldW) == &s[j])
        {
            cnt++;
            j += oldWlen - 1;
        }
    }
    if(cnt)
    {

        *result = (uint8_t *)malloc(j + cnt * (newWlen - oldWlen) + 1);

        j = 0;
        while (*s)
        {
            // compare the substring with the result
            if(OPT==f)
            {
                 if(cnt2==0)
                 {
                    if (StrStr(s, oldW) == s)
                    {

                            StrCpy(&((*result)[j]), newW);
                            cnt2=1;
                            j += newWlen;
                            s += oldWlen;
                    }
                    else
                    {
                        (*result)[j++] = *s++;
                    }
                 }
               else
                {
                    (*result)[j++] = *s++;
                }
            }
            else if(OPT==i)
            {
                if (StrStr(s, oldW) == s)
                    {

                            StrCpy(&((*result)[j]), newW);
                            j += newWlen;
                            s += oldWlen;
                    }
                    else
                    {
                        (*result)[j++] = *s++;
                    }
            }
            else if(OPT==l)
            {


                    if (StrStr(s, oldW) == s)
                    {
                       cnt2++;
                        if(cnt2==cnt)
                            {

                            StrCpy(&((*result)[j]), newW);
                            j += newWlen;
                            s += oldWlen;
                            }
                                 else
                            {
                                (*result)[j++] = *s++;

                            }
                    }
                    else
                    {
                        (*result)[j++] = *s++;

                    }
            }
         else
         {
            (*result)[j++] = *s++;
         }

        }

     (*result)[j] = 0;
    }
    else
    {
        iRet=RET_KO;
    }
 return iRet;
}
/*
  this function change string from lower case to upper case
  ex.paysky PAYSKY
*/
void StringUpper(uint8_t *szString, uint8_t slen)
{
	uint8_t index;

	for (index = 0; index < slen; index++)
	{
		if ( ((*(szString+index)) > 0x60) && ((*(szString+index)) < 0x7B) )
			*(szString+index) -= 0x20;
	}
}
void ProgFlow()
{
   uint8_t cmd[1000]={0};
	uint8_t sztemp[1000]={0};
    uint8_t FrmWord[100]={0};
	uint8_t toword[100]={0};
	uint8_t filedata[2000]={0};
	int16_t No_Files=0;
	uint8_t ** FILENAME=NULL;
	int16_t iRet=0,j=0;
	uint8_t OPT=1;
	uint8_t *result = NULL;

	///////////////////////////////
	fflush(stdout);
    gets((char*)cmd);
    if(cmd[strlen((char*)cmd)]!=' ')
    {
        cmd[strlen((char*)cmd)]=' ';
    }
    iRet=Parsing_command(cmd,FrmWord,toword,&FILENAME,&No_Files,&OPT);
    if(iRet==RET_OK)
    {
      StringUpper(FrmWord,strlen((const char*)FrmWord));
        for(j=0;j<No_Files;j++)
        {
            sprintf((char*)sztemp,"%s",FILENAME[j]);
            if(ReadFile(sztemp,filedata)==RET_OK)
            {
                switch (OPT)
                {
                     case i:
                     case l:
                     case f:
                        if(replaceWord(filedata,FrmWord, toword,&result,OPT)==RET_OK)
                        {
                         WriteFile(sztemp,result);
                         printf("File %d done\n",j+1);
                        }
                        else
                        {
                            printf("can not find word\n");
                            iRet=RET_KO;

                        }
                     break;
                     case (i|b):
                        if(StrStr(filedata,FrmWord))
                        {
                            Wirtebackupfile(sztemp,filedata);
                        }
                        if(replaceWord(filedata,FrmWord, toword,&result,i)==RET_OK)
                        {
                         WriteFile(sztemp,result);
                         printf("File %d done\n",j+1);

                        }
                        else
                        {
                            printf("can not find word\n");
                            iRet=RET_KO;

                        }
                         break;
                     case (l|b):
                         if(StrStr(filedata,FrmWord))
                            {
                                Wirtebackupfile(sztemp,filedata);
                                printf("File %d done\n",j+1);

                            }
                        if(replaceWord(filedata,FrmWord, toword,&result,l)==RET_OK)
                        {
                         WriteFile(sztemp,result);
                        }
                        else
                        {
                            printf("can not find word\n");
                            iRet=RET_KO;

                        }

                          break;

                     case (f|b):

                          if(StrStr(filedata,FrmWord))
                        {
                            Wirtebackupfile(sztemp,filedata);
                        }
                        if(replaceWord(filedata,FrmWord, toword,&result,f)==RET_OK)
                        {
                         WriteFile(sztemp,result);
                        }
                        else
                        {
                            printf("can not find word\n");
                            iRet=RET_KO;

                        }
                        break;
                     case (l|f):
                         if(replaceWord(filedata,FrmWord, toword,&result,l)==RET_OK)
                        {
                              memset(filedata,0x00,sizeof(filedata));
                              StrCpy(filedata,result);
                              free(result);
                              replaceWord(filedata,FrmWord, toword,&result,f);
                              {
                              WriteFile(sztemp,result);

                              }

                        }
                        else
                        {
                            printf("can not find word\n");
                            iRet=RET_KO;

                        }

                        break;
                     case (l|f|b):
                             if(StrStr(filedata,FrmWord))
                        {
                            Wirtebackupfile(sztemp,filedata);
                        }
                           if(replaceWord(filedata,FrmWord, toword,&result,l)==RET_OK)
                        {
                              memset(filedata,0x00,sizeof(filedata));
                              StrCpy(filedata,result);
                              free(result);
                              if(replaceWord(filedata,FrmWord, toword,&result,f)==RET_OK)
                              {
                              WriteFile(sztemp,result);

                              }
                               else
                            {
                                printf("can not find word\n");
                                iRet=RET_KO;

                            }
                        }
                        else
                        {
                            printf("can not find word\n");
                            iRet=RET_KO;

                        }
                        break;
                        default : iRet=RET_KO;
                        break;
                }
            }
            else
            {
                printf("please check that %s is exist\n",FILENAME[j]);
                iRet=RET_KO;

            }
            free(result);

        }

            for(j=0;j<No_Files+1;j++)
            {
                free(FILENAME[j]);
            }
                free(FILENAME);
      if(iRet==RET_OK)
      {
          printf("ALL DONE SUCCESSFULLY\n");

      }
      else
      {
         printf("Please check that the cmd is written in the right form\nplease read the manual to know how to write the cmd\n");

      }
    }
    else
    {
        printf("Please check that the cmd is written in the right form\nplease read the manual to know how to write the cmd\n");
    }
}
int main()
{   //LOCAL VARIABLES//
    ProgFlow();
    system("pause");
	return 0;
}
