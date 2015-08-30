#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>
#include "analyzer.hpp"
#include "createfile.hpp"
#include <stdio.h>

using namespace std;

class converter {
    char tag;
    string text; /* String objects - Easier for manipulation*/

    public:
            converter()
            {
                /* Haven't yet decided what to put here */
            }

            converter(char tags,string texts )
            {
                tag=tags;
                text=texts;
            }

            converter(string texts )
            {
                /* In case there are no need of tags. P tags are going to be used.*/
                text=texts;
            }

            string variable_detect()
            {
                 /*Used to detect variable presense and then remove them */
                int l=text.length();
                int i=l-1;
                /* Below for loop is used to ignore spaces at the end. */
                for(int j=i;j>=0;j--)
                {
                    if(text.at(j)==' ')
                        continue;
                    else
                    {
                        i=j; //Position where space ends and letter start.
                        break;
                    }
                }
                if(text.at(i)==']')
                {
                    int j=i;
                    while(text.at(j)!='['&&j!=0)
                    {
                        j--;
                    }
                    if(j==0)
                    {
                        /* No variables present. */
                        return NULL;
                    }
                    else
                    {
                        /* Returns the variable */
                        string variable=text.substr(j+1,i-j-1);
                        text=text.substr(0,j); /*Removes the variable from texts*/
                        return variable;
                    }

                }
                return "novarspresenthere";
            }

            string convert(string name, string section)
            {  //function overloading
                /* This method is invoked whenever a section is encountered */
                string convsec="";
                if(name.at(0)=='.') {
                    name=name.substr(1,name.length()-1);
                    convsec="<div class=\""+name+"\">"+section+"</div>";
                } else {
                 convsec="<div id=\""+name+"\">"+section+"</div>";
                 }
                return convsec;
            }

            string convert()
            {
                /* Exit the convert function if the text is empty to prevent crashing */
                /* This function checks the tag passed and converts the text to HTML */
                string result;
                if(text.empty()) {
                    return "";
                }
                string vari=variable_detect();

                if(!(vari.empty()||vari.compare("novarspresenthere")==0))
                {
                    result="<div id=\""+vari+"\">";

                }
                /* The 3 lines below are used in parsing links */
                string linkloc,anc;
                int sha=0,k;
                k=text.length()-7;
                Equip_Necessary imgmover;
                switch(tag)
                {
                    case 'b':   result+="<strong>"+text+"</strong>";
                                break;
                    case 'i':   result+="<em>"+text+"</em>";
                                break;
                    case 's':   result+="<strike>"+text+"</strike>";
                                break;
                    case 'c':   result+="<pre>"+text+"</pre>";
                                break;
                    case 'm':   result+="<img src=\"media\\"+text+"\">\n";
                                imgmover.move_media(text);
                                break;
                    case '1':   result+="<h1>"+text+"</h1>\n";
                                break;
                    case '2':   result+="<h2>"+text+"</h2>\n";
                                break;
                    case '3':   result+="<h3>"+text+"</h3>\n";
                                break;
                    case '4':   result+="<h4>"+text+"</h4>\n";
                                break;
                    case '5':   result+="<h5>"+text+"</h5>\n";
                                break;
                    case 'p':   result+="<p>"+text+"</p>\n";
                                break;
                    case 'l':   /* For links */
                                while(k>0) {
                                    if(text[sha]==':'&&text[sha+1]==':'&&text[sha+2]=='-'&&text[sha+3]==':'&&text[sha+4]==':') {
                                        /* Successful link*/
                                    result+="<a href=\""+linkloc+"\">"+anc+"</a>";
                                    }
                                }
                                break;
                    case 't':   /* For tables */
                               {
                                  string iserror=parsetable(text);
                                //if(!iserror.compare("err")) Error detection code - for future use
                                result=result+parsetable(text);
                               }
                                break;
                    default:    break;
                }

                if(!(vari.empty()||vari.compare("novarspresenthere")==0)) {
                    result+="</div>\n";

                }
                return result;
            }

            string askpolitely(char a, string b)
            { //set values for tag and string
                tag=a;
                text=b;
                string ans = this->convert();
                return ans;
            }
            string askpolitely(char a,string linkloc, string anchortext) {
                tag=a;
                text=linkloc+"::-::"+anchortext;
                string ans = this->convert();
                return ans;
            }

            string parsetable(string text) {
                /* Parses the table */
                cout<<"From parse table->"<<text<<endl;
                int tablelength=text.length();
                int i=0,j=0,k=0;
                string content; //Content between each '|'
                string overall="<table>"; //HTML + content
                bool isdash; //true if content is a dash
                bool haz=false; //flag which tells if there is content to be unloaded

                /* To check if the table has headers */
                int l=0,m=0; bool hashead=false;
                    while(text[l]!='\n') l++;
                    if(text[l]=='\n') { //First instance of \n
                        l=l+1;
                        if(text[l+1]=='-'&&text[l+2]=='-'&&text[l+3]=='-')
                            hashead=true;
                    }
                    else {
                        /* In this case, the stupid user gives one line table. I mean,
                        who even does that? The user is like, really really stupid. */
                        return "err";
                    }
                if(hashead) overall+="\n<thead>";
                else overall+="\n<tr>";

                /* To simply decode a table without a head*/
                while(text[i]) {
                        //overall+="<tr>\n";
                    if(text[i]=='|') {
                            if(text[i+1]=='\n') {
                                    i=i+2;
                                    if(hashead) {
                                            overall+="\n</thead>";
                                            hashead=false;}
                                    else { if(isdash==false) overall+="</tr>";}
                                     if(isdash==false) overall+="\n<tr>";
                            }
                            if(i+1<tablelength)
                        if(isdash==false) overall+="<td>";
                        j=i+1;
                        while(text[j]!='|'&&j<tablelength) {
                            j++;
                        }
                        if(text[j]=='|') {
                            content=text.substr(i+1,j-i-1);
                            cout<<"\ncontent="<<content;
                            if(content.compare("---")) {
                            overall+=content;
                            overall+="</td>";
                            isdash=false;
                            }
                            else {isdash=true;}
                            haz=false;
                        }else if(haz){
                            return "err";
                        }
                    }
                    cout<<"\ni = "<<i<<"\nj = "<<j<<"\nLength = "<<tablelength<<"\n--------\n";
                    i=j;haz=(i+1<tablelength)?true:false;
                }
                overall+="</tr></table>";
                return overall;
            }




};

/* This class is used to provide HTML header, footer and style to the output files. */

class provide_htmlcss {

    public:
    friend string getCurrentDirectory();

        provide_htmlcss() {
        }

        std::string header(std::string title){
            std::string head="<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<title>"+title+"</title>\n<!-- Bootstrap -->\n<link href=\"css/bootstrap.css\" rel=\"stylesheet\">\n<link href=\"css/style.css\" rel=\"stylesheet\">\n<script src=\"js/jquery-1.11.2.js\"></script>\n<script src=\"js/bootstrap.js\"></script>\n<!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->\n<!-- WARNING: Respond.js doesn't work if you view the page via file:// -->\n<!--[if lt IE 9]>\n<script src=\"js/html5shiv.js\"></script>\n<script src=\"js/respond.js\"></script>\n<![endif]-->\n</head>\n<body><div class=\"container\">\n";
            return head;
        }

        std::string footer() {
            std::string footer = "\n<credits> This webpage is generated by the ALANG Markdown to HTML converter. Not for commerical use.</credits></div></body></html>";
            return footer;
        }

        void File_append(string str)  {        //This function appends
            ofstream file2;
            if(str.compare("/*\n\tStyle.css Generated by ALANG \n*/\n\ncredits{ \n\ttext-align:center;\n}\ntable{\n\tborder:1px solid #000;\n}\ntd,th{\n\tborder:1px solid #000;\n\tpadding:10px;\n}\n")==0){
                    file2.open("style.css",ios::out);
            } else {
            file2.open("style.css",ios::app);
            }
            file2<<str<<endl; //the string str to file
            file2.close();
        }


};
