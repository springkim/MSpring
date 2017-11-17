//
// Created by kimbom on 17. 11. 11.
//

#ifndef ISPRING_GNUZIP_H
#define ISPRING_GNUZIP_H
#ifdef ISPRING_LINUX
#include<iostream>
#include<vector>
#include<atomic>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ptrace.h>		//ptrace ,PTRACE_TRACEME
#include<sys/wait.h>			//wait4
#include<sys/resource.h>	//struct rusage
#include<sys/user.h>			//struct user_regs_struct
#include<fcntl.h>
#include"../File/FileManager.h"
namespace ispring_3rdparty{
	std::string MakeTempZipFile(){
		char buf[32];
		struct timeval val;
		struct tm* ptm;
		gettimeofday(&val, NULL);
		ptm = localtime(&val.tv_sec);
		sprintf(buf, "/tmp/fd_%02d%02d%05ld", ptm->tm_min, ptm->tm_sec, val.tv_usec);
		return buf;
	}
	void ZipWithProgress(std::vector<std::string> args,std::atomic<int>* progress,int max_progress){
		std::string fd_stdout=MakeTempZipFile();
		pid_t pid=fork();
		if(progress!=nullptr) {
			(*progress) = 0;
		}
		if(pid!=0){
			int status;
			struct rusage ruse;
			struct user_regs_struct reg;
			int64_t sz=0;
			int cnt=-2;
			while(1) {
				wait4(pid, &status, 0, &ruse);
				if(WIFEXITED(status))break;
				ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
				int64_t nsz=ispring::File::FileSize(fd_stdout);
				if(sz!=nsz){
					sz=nsz;
					cnt++;
					if(progress!=nullptr) {
						(*progress) = std::max((cnt / 2) * 100 / max_progress, 0);
					}
				}
			}
			remove(fd_stdout.c_str());
			if(progress!=nullptr) {
				(*progress) = 100;
			}
		}else{
			ptrace(PTRACE_TRACEME,0,NULL,NULL);
			int fd = open(fd_stdout.c_str(), O_WRONLY|O_CREAT, 0666);
			dup2(fd,1);
			close(fd);
			if(args.size()==7){
				execlp(args[0].c_str(),args[1].c_str(),args[2].c_str(),args[3].c_str()
				,args[4].c_str(),args[5].c_str(),args[6].c_str(),NULL);
			}else if(args.size()==5){
				execlp(args[0].c_str(),args[1].c_str(),args[2].c_str(),args[3].c_str()
						,args[4].c_str(),NULL);
			}
		}
	}
	void UnzipWithProgress(std::vector<std::string> args,std::atomic<int>* progress,int max_progress){
		std::string fd_stdout=MakeTempZipFile();
		pid_t pid=fork();
		if(progress!=nullptr) {
			(*progress) = 0;
		}
		if(pid!=0){
			int status;
			struct rusage ruse;
			struct user_regs_struct reg;
			int64_t sz=0;
			int cnt=-1;
			while(1) {
				wait4(pid, &status, 0, &ruse);
				if(WIFEXITED(status))break;
				ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
				int64_t nsz=ispring::File::FileSize(fd_stdout);
				if(sz!=nsz){
					sz=nsz;
					cnt++;
					if(progress!=nullptr) {
						(*progress) = std::max((cnt) * 100 / max_progress, 0);
					}
				}
			}
			remove(fd_stdout.c_str());
			if(progress!=nullptr) {
				(*progress) = 100;
			}
		}else{
			ptrace(PTRACE_TRACEME,0,NULL,NULL);
			int fd = open(fd_stdout.c_str(), O_WRONLY|O_CREAT, 0666);
			dup2(fd,1);
			close(fd);
			if(args.size()==5){
				execlp(args[0].c_str(),args[1].c_str(),args[2].c_str(),args[3].c_str()
						,args[4].c_str(),NULL);
			}
		}
	}
}
#endif		//ISPRING_LINUX
#endif //ISPRING_GNUZIP_H
