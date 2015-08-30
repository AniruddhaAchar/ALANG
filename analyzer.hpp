#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>
#include "convertor.hpp"
#include "createfile.hpp"

using namespace std;

/* Above line imports this file only once */
class findall                                             //This is the searching class
{
public:
    string end_s(string s,int &i,char tag)                //takes in current position of i, character array and the ending tag
    {
        string temp="";
        i++;
        while(s[i]!=tag)                                      //searches till end of the code
        {
            temp+=start_s(s,i);                               //checking for nested tags. If non found the current character is appended with the temp string
        }
        return temp;
    }
    virtual string start_s(string s,int &i)=0;            //pure virtual function as the tags are unknown yet

};

class Tags : public findall
{

    bool p,b,h,it,co,sc,v;                                   //flags to find where the check starting and ending of tags

public:
    Tags()
    {
        p=false;
        b=false;
        it=false;
        h=false;
        v=false;
        co=false;//initialize all tags to false initially using the constructor

    }
    string start_s(string s,int &i)
    {
        converter c;                                      //object of converter class
        string st;                                        //this string is created in every cycle of the search
        char ch=s[i];
        switch (ch)
        {
        case '\n':
            getTagcontent(i,'\n',p,s,st);       //paragraph is isolated from the tags
            st=c.askpolitely('p',st);
            p=false;                                  //  html paragraph tags are added to the

            break;
        case  '*':
            getTagcontent(i,'*',b,s,st);        //bold is isolated from the tags
            st=c.askpolitely('b',st);             //html strong tags are added to the
            //b=false;

            break;
        case  '`':
            getTagcontent(i,'`',co,s,st);       //code is isolated from the tags
            st=c.askpolitely('c',st);             //html code tags are added to the
            //co=false;
            break;
        case  '~':
            getTagcontent(i,'~',it,s,st);       //italics is isolated from the tags
            st=c.askpolitely('i',st);             //html italics tags are added to the
            //it=false;
            break;
        case '#':
            getTagcontent(i,s,st);              //The getTagcontent method is overloaded for head tags and the css part
            break;
        case '!':
            getimgLink(i,s,st);
            /* Aniruddha will add a function here to
            call askpolitely('m',imagelocation) for images
            and askpolitely('l',linkaddress,anchortext) for links
            */
            break;            //For image and link
        case '$':
        {
            getSection(s,i,st);

        }
        break;
        default:
            st+=s[i];
            i++;

        }
        return st;                                        //returns the converted string back

    }
    void getTagcontent(int &i,char endtg,bool &flag,string s,string &st)
    {
        if(flag==false)
        {
            flag=true;
            st+=end_s(s,i,endtg);                     //searches the
            //i++;

        }
        else
        {

            flag=false;
            i++;
            //st+=s[i];

        }
    }
    void getTagcontent(int &i, string s,string &st)
    {
        i++;
        converter c;
        provide_htmlcss addcss;
        char ch=s[i];
        switch(ch)
        {

        case '1':
            st+=end_s(s,i,'\n');
            st=c.askpolitely(ch,st);
            //pass the resulting string to h1 conversion
            break;
        case '2':
            st+=end_s(s,i,'\n');
            st=c.askpolitely(ch,st);
            //pass the resulting string to h2 conversion
            break;
        case '3':
            st=end_s(s,i,'\n');
            st=c.askpolitely(ch,st);
            //pass the resulting string to h3 conversion
            break;
        case '4':
            st=end_s(s,i,'\n');
            st=c.askpolitely(ch,st);
            //pass the resulting string to h4 conversion
            break;
        case '5':
            st=end_s(s,i,'\n');
            st=c.askpolitely(ch,st);
            //pass the resulting string to h5 conversion
            break;
        case '6':
            st=end_s(s,i,'\n');
            st=c.askpolitely(ch,st);
            //pass the resulting string to h6 conversion
            break;
        case '.':
        case '#'://for lexing CSS
        {
            string css;
            while(s[i]!='}')
            {
                css+=s[i];
                i++;
            }
            css+=s[i];
            addcss.File_append(css);
            //send css to askpolitely
        }
        i++;
        break;
        case 't':
        {
            string table;
            i++;
            i++;//this is to remove the next line character
            while(s[i]!='%')
            {
                table+=s[i];
                i++;
            }
            i++;
            table=table.substr(0,table.length()-1);
            st=c.askpolitely('t',table);
        }
        break;

        }
    }
    void getSection(string s, int &i,string &st)
    {   converter d;
        if(v==false)
        {
            string variable="";
            cout<<"Section detected"<<endl;
            i++;
            while(s[i]!=':')
            {
                variable+=s[i];
                i++;
            }
            i++;
            cout<<"Variable found and ="<<variable<<endl;
            getTagcontent(i,'$',v,s,st);
            cout<<"End of section\n";
            //call askpolitely with sections here
            st=d.convert(variable,st); // is this code right?
        }
        else
        {
            cout<<"From else\n";
            v=false;
            i++;
            return;
        }

    }
    void getimgLink(int &i,string s,string &st)
    {
        string anchor="",link="",img="";
        converter c;
        i++;
        switch(s[i])
        {
        case '('://start of link lexing
        {
            i++;
            while(s[i]!=')')
            {
                anchor+=s[i];
                i++;
            }
            i++;
            try
            {
                if(s[i]!='[')
                    throw 1;
                else
                {
                    i++;
                    while(s[i]!=']')
                    {
                        link+=s[i];
                        i++;
                    }
                    cout<<"Link="<<link<<endl;
                }
            }
            //pass link and anchor to converter
            catch(int exep)
            {
                if(exep)
                    cout<<"Link missing \n";
            }
            i++;
            st=c.askpolitely('l',link,anchor);
            cout<<"After conversion\n";
        }
        break;//end of the link
        case '{':
        {
            i++;
            img;
            while(s[i]!='}')
            {
                img+=s[i];
                i++;
            }
            st=c.askpolitely('m',img);
            i++;
        }
        break;
        }
    }
};
