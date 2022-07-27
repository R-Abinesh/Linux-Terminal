#include<iostream>
#include<string>
#include<vector>
#include<unistd.h>
#include<cstring>
#include<sstream>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

using namespace std;


void hstcmd(vector<string>hst,int i){
	i = hst.size()-i;
	if(!hst.empty() && i>-1 ){
		cout<<hst[i]<<endl;
		hst.pop_back();
	}
	else{
		;
	}
}

void str_par(string s, string &cmd, string &param, char delimiter=' '){
	
/*DEMO 
	stringstream ss(s);
	string word;
	while(ss>>word){
		cout<<word<<endl;

	}
DEMO END
*/



	int start = 0;
	int end = s.find(delimiter);
	cmd = s.substr(start,end);

	if(end ==-1){
		param = "";
		return; 
	}
	
	start = end+1;
	param = s.substr(start,s.size());

}


void list(string param)
{
	char cwd[256];
	getcwd(cwd,256);

	struct dirent *entry;

	/*struct dirent {
		ino_t d_ino;		 // inode number 
		off_t d_off;		 // offset to the next dirent 
		unsigned short d_reclen; // length of this record 
		unsigned char d_type;	 // type of file 
		char d_name[256]; 	 // filename 
	 }; */

	DIR *dStream = opendir(cwd);
	int ret=0;
	int l=0,a=0;
	if(param != "" && param[0] == '-'){
		if(param.find('l')!=string :: npos)
			l++;
		if(param.find('a') != string::npos)
			a++;
		else{
			cout<<"ls "<<param<<" Not found"<<endl;
			return;
		}
	}
	else if(param !=""){
		cout<<"ls "<<param<<" Not found"<<endl;
		return;
	}
	while((entry = readdir(dStream)) !=NULL ){
		if(entry->d_name[0] == '.'&&  a!=1 )
			continue;
		else
			 cout<<entry->d_name<<"\t\t";
	}
	if(ret)
		perror("ls");
	closedir(dStream);
	cout<<endl;

}

int isDir(string dir_name){
	DIR *directory = opendir(dir_name.data());
	if(directory != NULL){
		//cout<<"Directory"<<endl;
		return 0;
	}
	else if(errno == ENOTDIR){
		//cout<<"Normal file"<<endl;
		return 1;
	}
	else{
		return -1;
	}
}

void ch_dir(const char* dir)
{       int ret = chdir(dir);
        if(ret)
                perror("Cannot move to the directory ");
}


void finder(const char* cur_dir,string f_name){
	ch_dir(cur_dir);
	DIR *dStream = opendir(cur_dir);
	struct dirent *entry;
	if(dStream != NULL){
		while((entry = readdir(dStream))!=NULL){
			if(entry->d_name[0]=='.'){
				continue;
			}
			else if (isDir(entry->d_name)==0){ 
				if(entry->d_name == f_name){
					cout<<cur_dir<<"/"<<f_name<<" is a folder"<<endl;
					break;
				}
				string newDir = (string)cur_dir+"/"+entry->d_name;
				finder(newDir.data(),f_name);
				ch_dir("..");		
			}
		
			//else if(isDir(entry->d_name) == 1){
			else{
			       	if(entry->d_name == f_name){
					cout<<cur_dir<<"/"<<f_name<<endl;
					break ;
				}
			}
		//	else if(isDir(entry->d_name)== -1){
		//		break;
		//	}
			
		}
	}
	closedir(dStream);
}
/*
void ch_dir(const char* dir)
{       int ret = chdir(dir);
	if(ret)
		perror("Cannot move to the directory ");
}*/

void make_dir(string s, mode_t mode= 0777)
{
	string first,second;
	str_par(s,first,second);
	
	while(second!=""){
		int ret = mkdir(first.data(),mode);
		if(ret)
			perror("Failed to create new directory");

		str_par(second,first,second);
	}

	int ret = mkdir(first.data(),mode);
	if(ret)
		perror("Failed to create new directory ");
}

void rem_dir(string s)
{
	string first,second;
	str_par(s,first,second);
	while(second!=""){
		int ret = rmdir(first.data());
		if(ret)
			perror("Failed to remove directory");
		str_par(second,first,second);
	}
	int ret = rmdir(first.data());
	if(ret)
		perror("Failed to remove directory :");
}

void run(string bin_exec_file){
	string path="/bin/"+bin_exec_file;
	int ret;
	ret = execl(path.data(),bin_exec_file.data(),NULL);
	if(ret)
		perror("");
	
}

void cat(string param){
	char s[1024];
	string first,second;
	str_par(param,first,second);
	while(second!=""){
		FILE *fptr = fopen(first.data(),"r");
		if(!fptr){
			perror("");
			return ;
		}
		cout<<first<<endl;
		while(fgets(s,1024,fptr)){
			cout<<s;	
		}
		cout<<"-------------------------------------------------------------------------------"<<endl<<endl;
		str_par(second,first,second);
	}
	cout<<first<<endl;
	FILE *fptr = fopen(first.data(),"r");
	if(!fptr){
		perror("");
		return;
	}
	while(fgets(s,1024,fptr))
		cout<<s;


}


int main(){
	chdir(getenv("HOME"));
        vector<string>hst;
	int p=0;
	string s;
	char *pwd;
	string own = "Root@Pc~";
	while(s!="exit"){
		getcwd(pwd,256);
		cout<<own<<" "<<(string)pwd<<"# ";
		getline(cin,s);
		if(s=="clear"){
                	 system("clear");
			 continue;
		}

		if(s[0] == 27 && s[1]==91){       // 27 denotes the ASCII value for ESC (glyph notion = '^[' )  and 91 denotes the ascii value for open bracket '['
			if(s[2]==65){			// 65 for A ->  up arrow   = ^[[A
				p++;
				hstcmd(hst,p);
			//	cout<<"Up arrow"<<endl;
			}
			else if(s[2]==66)		 // 66 for B -> down arror  = ^[[B
				cout<<"Down arrow"<<endl;
			else if(s[2]==67)		 // 67 for C -> left arrow  = ^[[C
				cout<<"Left arrow"<<endl;
			else if(s[2]==68)		 // 68 for D -> right arrow = ^[[D  
				cout<<"Right arrow"<<endl;
		}
		else if(s!=""){
			p=0;
			hst.push_back(s);

			string cmd;
		       	string param;
		    	//string attr;	
			str_par(s, cmd, param);   //String parsing
			
			if(cmd == "ls"){
				list(param);
				continue;
			}
			

			const char *dir = param.data();
			if(cmd == "cd"){	 //change directory
				ch_dir(dir);
			}
			else if(cmd == "find"){
				finder(pwd,param);
				//isDir(param);
				//cout<<param<<endl;
			}
			else if(cmd == "pwd"){   //current working directory
				cout<<pwd<<endl;
			}
			
			else if(cmd == "mkdir"){
				make_dir(dir);
			}

			else if(cmd == "rmdir"){
				rem_dir(dir);
			}

			else if(cmd=="run"){
				run(dir);
			}

			else if(cmd == "cat"){
				cat(param);
			}
			
			else{
				cout<<s<<" :Command not found"<<endl;
			}

		}
		

	}
	

}
