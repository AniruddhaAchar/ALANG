#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>
#include "aniruddha.hpp"
#include "abhilash.hpp"

using namespace std;

void replaceAll( string &s, const string &search, const string &replace )      //Replaces SubString
{
    for( size_t pos = 0; ; pos += replace.length() ) {
        // Locate the substring to replace
        pos = s.find( search, pos );
        if( pos == string::npos ) break;
        // Replace by erasing and inserting
        s.erase( pos, search.length() );
        s.insert( pos, replace );
    }
}
string getCurrentDirectory()
{
    unsigned long maxDir = 260;
    char currentDir[maxDir];
    GetCurrentDirectory(maxDir, currentDir);
    return string(currentDir);
}
string imagout,ip1,op1,cp1,dr;
class Equip_Necessary
{
    string str1,str2,str3,dir,str;
    void CreateFolder(char *path)                               //Directory Creator Function
    {
        if(!CreateDirectory(path,NULL))
        {
            cout<<"Directory Creation Failed";
            exit(0);
        }
        else
            cout<<"Directory Creation Successful\n";
    }
    public:
    friend void replaceAll();
    friend string getCurrentDirectory();
    void create(string path)                        //Creates Directories and moves all the constant files
    {
        str=path;                                 // necessary for website in the path directory passed as parameter
        char *ch=&str[0];
        CreateFolder(ch);
        str+="\\\\";

        str1=str+"css";
        char *c1=&str1[0];
        CreateFolder(c1);

        str3=str+"media";
        char *c3=&str3[0];
        CreateFolder(c3);
        imagout=str3+"\\\\";                              //imagout refers to the user difined directory

        str1+="\\\\";
        string Input1="C:\\website\\bootstrap.css";
        string CopiedFile1="bootstrap.css";
        string OutputFolder1=str1;
        CopyFile(Input1.c_str(),string(OutputFolder1+CopiedFile1).c_str(), TRUE);

        dir=getCurrentDirectory();
        replaceAll(dir,"\\","\\\\");
        dir+="\\\\";
        dr=dir;

        string Input4=(dir+"\\style.css");
        string CopiedFile4="style.css";
        ip1=Input4;
        op1=OutputFolder1;
        cp1=CopiedFile4;

        str2=str+"js";
        char *c2=&str2[0];
        CreateFolder(c2);

        str2+="\\\\";
        string Input2="C:\\website\\bootstrap.js";
        string CopiedFile2="bootstrap.js";
        string OutputFolder2=str2;
        CopyFile(Input2.c_str(),string(OutputFolder2+CopiedFile2).c_str(), TRUE);

        //Copying File, Jquery.js to JS Directory
        string Input3 = "C:\\website\\jquery-1.11.2.js";
        string CopiedFile3 = "jquery-1.11.2.js";
        CopyFile(Input3.c_str(), string(OutputFolder2+CopiedFile3).c_str(), TRUE);

    }
    void move_media(string img)
    {
        string OutputFolder3=imagout;
        char* imag=&img[0];                                         //File checking needs char array
        fstream file(imag);
        if (file)
        {
            string Input5=(dr+"\\"+img);
            string CopiedFile5=img;
            CopyFile(Input5.c_str(),string(OutputFolder3+CopiedFile5).c_str(), TRUE);
        }
        else
        {
            cout<< "File" << imag<<" not found.\n";
            return;
        }
    }
    void remove_files()
    {
        /*string file1=dir+"\\input.alg";
        char *ch1=&file1[0];*/
        string file2=dir+"\\userfile.txt";
        char *ch2=&file2[0];
        string file3=dir+"\\index.html";
        char *ch3=&file3[0];
        string file4=dir+"\\style.css";
        char *ch4=&file4[0];
        //int res1=remove(ch1);
        int res2=remove(ch2);                            //result stores False when the file is removed
        int res3=remove(ch3);
        int res4=remove(ch4);
        if(/*res1||*/res2 && res3 && res4)
        cout<<"File Deletion Failed!\n";
        else
        cout<<"File Deleted Successfully\n";
    }
};

void move_file(string dir, ofstream &file)
{
    CopyFile(ip1.c_str(),string(op1+cp1).c_str(), TRUE);
    dir+="\\\\";
    string direc;
    direc=getCurrentDirectory();
    replaceAll(direc,"\\","\\\\");
    direc+="\\\\";
    string Input4=direc+"\\\\Index.html";
    string CopiedFile4="Index.html";
    string OutputFolder=dir;
    CopyFile(Input4.c_str(),string(OutputFolder+CopiedFile4).c_str(), TRUE);
}

std::string getinputandsend(string path)
{
    string text;
    char *cha=&path[0];
    ifstream fin("input.alg", ios::in);
    ofstream fout("userfile.txt", ios::out);
    while(getline(fin,text))
    {
        if(text.empty())
        fout<<endl;
        else
        {
            replaceAll(text,"\'","\\\'");
            replaceAll(text,"\'","\\\"");
            fout<<text<<endl;
        }
    }
    fin.close();
    fout.close();
    ifstream file3("userfile.txt", ios::in);                 //This Edited File is being sent as String
    stringstream buffer;
    buffer << file3.rdbuf();
    text=buffer.str();
    file3.close();
    cout<<"\nSent text: "<<text<<endl;
    return text;
}
void progbar(int i)
{
    static int prog=0;
    string ch;
    cout<<"Please Wait, while your Website is being generated.\n";
    while(prog<=i)
    {
        if(prog<=10)
        ch="\r[==|>";
        if(prog<=20 && prog>10)
        ch="\r[===|>";
        if(prog<=30 && prog>20)
        ch="\r[====|>";
        if(prog<=40 && prog>30)
        ch="\r[=====|>";
        if(prog<=50 && prog>40)
        ch="\r[======|>";
        if(prog<=60 && prog>50)
        ch="\r[=======|>";
        if(prog<=70 && prog>60)
        ch="\r[========|>";
        if(prog<=80 && prog>70)
        ch="\r[==========|>";
        if(prog<=90 && prog>80)
        ch="\r[===========|>";
        if(prog<=100 && prog>90)
        ch="\r[============|>";
        cout<<ch<<prog<<"% completed";
        Sleep(1);
        prog++;
    }
}
