#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


int isVowel(char c){
	return c == 'a' || c == 'A'|| c == 'e'|| c == 'E'|| c == 'i'|| c == 'I'|| c == 'o'|| c == 'O'|| c == 'u'|| c == 'U';

}



int main(int argc, char **argv){
	int filenoElenco = 1;
	int filenoCopia = 1;
	int filenoCheck = -1;
	int pidTemp; //pid temporaneo per creare due figli
	
	//apertura del file da copiare
	if((filenoElenco = open("ELENCO.TXT", O_RDONLY)) == -1){
		fprintf(stderr,"Errore nell'apertura di elenco\n");
		exit(0);
	}
	//apertura del file copia
	if((filenoCopia = open("copia.txt", O_CREAT | O_TRUNC | O_APPEND, 0644)) == -1){
		fprintf(stderr, "Errore nella creazione di copia\n");
		exit(0);
	}
	printf("%lu", write(filenoCopia, "a", 1));
	//rimozione del file di controllo nel caso esistesse ancora
	unlink("check");
	//creazione primo figlio
	pidTemp = fork();
	
	
	if(pidTemp == 0){
		char c; //"buffer" per leggere elenco
		int pid = fork(); //pid == 0 per il copiatore di consonanti, > 0 per il copiatore di vocali
		while(1){
			if(pid == 0){
				while(access("check", F_OK) == 0);  //opera quando il file non esiste
				if(read(filenoElenco, &c, 1) <= 0){
					fprintf(stderr, "addio consonantiere\n");
					exit(0);
				}
				fprintf(stderr, "Consonantiere: %c\n", c);
				if(!isVowel(c)){
					printf("%lu", write(filenoCopia, &c, 1));
					write(filenoCopia, &c, 1);
				}
				lseek(filenoElenco, -1, SEEK_CUR);
				open("check", O_CREAT, 0744);
			}
			else{ //il pid è maggiore di 0
				while(access("check", F_OK) ==-1); //opera quando il file esiste
				if(read(filenoElenco, &c, 1) <= 0){
					fprintf(stderr, "addio vocaliere\n");
					unlink("check");
					exit(0);
				}
				fprintf(stderr, "Vocaliere: %c\n", c);
				if(isVowel(c)){
					printf("%lu", write(filenoCopia, &c, 1));
				}
				unlink("check"); //resetta
			}
		}
		
	}
	else{
		printf("Addio padre\n");
		exit(0); //il padre originale cessa di esistere
	}
	
}
