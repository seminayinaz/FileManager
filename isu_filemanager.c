#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAXC 1024
#define MAXCH 2500

int fileCheck(const char *fileName){
  //Bir dosyanin mevcut olup olmadigini kontrol eder.
    if(!access(fileName, F_OK )){
        return 1;
    }
    else{
        return 0;
    }
}

void create_file(char name_of_file[]){
  //Bir dosya verilen dizinde mevcut degilse olusturur, mevcutsa hata verir.
    FILE *p_file;
    if ((p_file = fopen(name_of_file, "r")) != NULL){
        printf("Bu isimde bir dosya zaten mevcut.\n");
    }
    else if ((p_file = fopen(name_of_file, "w")) != NULL)
        printf("Dosya basariyla olusturuldu.\n");

    fclose(p_file);
}

void remove_file(char name_of_file[]){
  //Dosya mevcutsa siler, mevcut degilse hata verir.
  if(fileCheck(name_of_file)){
    if (remove(name_of_file) == 0){
      printf("Dosya basariyla silindi.\n");
    }
    else{
      printf("Dosya silinemedi.\n");
    }
  }
  else{
    printf("Dosya mevcut degil. Lutfen mevcut bir dosya girin.\n");
  }
}

int copy_file(char name_of_file[],char name_of_file2[]){
  //Mevcut olan bir dosyayi, mevcut olan/olmayan başka bir dosyaya kopyalar.
  FILE *p_file, *p_file2;
  char c;
  
  if((p_file = fopen(name_of_file, "r")) == NULL){
    printf("Dosya bulunamadi : %s\n", name_of_file);
    return 0;
  }

  if ((p_file2 = fopen(name_of_file2, "w")) == NULL) {
    strcat(name_of_file2, "/");
    strcat(name_of_file2, name_of_file);
    p_file2 = fopen(name_of_file2, "w");
  }

  while ((c = fgetc(p_file)) != EOF) {
    fputc(c, p_file2);
  }
  printf("%s dosyasina kopyalandi \n", name_of_file2);
  
  fclose (p_file);
  fclose (p_file2);
  return 0;
}

void move_file(char name_of_file[],char name_of_file2[]){
  //Bir dosyayi bir dizinden baska bir dizine kopyalar ve eski dizindeki dosyayi siler.
  strcat(name_of_file2, "/");
  strcat(name_of_file2, name_of_file);
  copy_file(name_of_file, name_of_file2);
  remove(name_of_file);
  printf("%s dosyasina tasindi.\n", name_of_file2);
}

void rename_file(char name_of_file[],char new_name[]){
    //Bir dosyayi, mevcut dizinde ayni isme sahip olmayan bir isimle yeniden adlandirir.
    int ret;
    if(fileCheck(name_of_file)){
        if(!fileCheck(new_name)){
        ret = rename(name_of_file, new_name);
        if(ret == 0) {
            printf("Dosya yeniden adlandirildi.\n");
        }
        else {
            printf("Error: Dosya yeniden adlandirilamiyor.\n");
        }
        }
        else{
        printf("%s dosyasi zaten mevcut. Lutfen baska bir dosya adi secerek yeniden deneyin.\n",new_name);
        }
    }
    else{
        printf("Dosya mevcut degil. Lutfen mevcut bir dosya girin.\n");
    }
}

int isTxtFile(char *fullName){
    //Bir dosyanin txt dosyasi olup olmadigini kontrol eder.
    char* ext;
    char* p;
    char a[]=".txt";
    ext = strchr(fullName,'.');
    if(strcmp(ext,a)==0){
        return 1;
    }
    else{
        printf("Bu bir txt dosyasi degil. Lutfen bir txt dosyasiyla deneyin.\n");
        return 0;
    }
}

void add_text(char name_of_file[]){
    //Eger bir dosya txt dosyasi ise verilen metni o dosyanin sonuna ekler.
    char text2[MAXC];
    FILE *p_file;
    p_file=fopen(name_of_file,"a++");
    if(p_file==NULL){
        printf("Dosya acilamadi.\n");
        exit(1);
    }
    else if(isTxtFile(name_of_file)){
        printf("Eklenecek metni girin :\n");
        fputs(fgets(text2,MAXC,stdin),p_file);
        fputs("",p_file);
        rewind(p_file);
        fgets(text2,MAXC,p_file);
        printf("Metin eklendi.\n");
    }  
    fclose(p_file);
}

void remove_content(char name_of_file[]){
    //Verilen dosya mevcutsa icerigini siler. Degilse hata verir.
    FILE *fp;
    if (fileCheck(name_of_file)){
        fp=fopen(name_of_file,"w");
        if ((fp!= NULL)) {
            printf("Dosya icerigi silindi.\n");
        }
        else{
        printf("Error : Dosya acilamadi.\n");
        }
    }
    else{
        printf("Dosya mevcut degil.\n");
    }
    fclose(fp);
}

void insert_text(char name_of_file[],int index){
  //Bir dosyada verilen bir konuma metin ekler. 
  char first[1000], text_str[MAXCH], end[1000], ch;
  int i=0;
  printf("Eklemek istediğiniz metni giriniz.\n");
  FILE *p_file ;
  p_file = fopen(name_of_file, "r");
  fgets(text_str,MAXCH,stdin);

  if(p_file == NULL){
    printf("Dosya acilmadi.\n");
  }
  else{
    while(index > i){
      ch = fgetc(p_file);
      first[i] = ch;
      i++;
    }
    fseek(p_file, index, SEEK_SET);
    i = 0;
    while((ch = fgetc(p_file)) != EOF){
      end[i] = ch;
      i++;
    }
    fclose(p_file);
    p_file = fopen(name_of_file, "w");
    fputs(first, p_file);
    fputs(text_str, p_file);
    fputs(end, p_file);
    printf("Metin eklendi\n");
    fclose(p_file);
  }
}

int main(void) {
    /*
    Kullanicidan gelen istekleri space'lerden bolerek bir arraye atar ve girilen komutlara gore istenilen fonksiyonlar calisir.
    */
    int index;
    char text2[MAXC] ;
    char text[1000], c;
    char name_of_file[50], name_of_file2[50], new_name[50];
    int num = 0;
    char a[MAXC] = "";
    char *create = "create";
    char *remove = "remove";
    char *rename = "rename";
    char *copy = "copy";
    char *move = "move";
    char *addtext = "add_text";
    char *removeContent = "remove_content";
    char *insert = "insert";
    char *help = "/h";
    char komut[100],sorgu[100];
    char a2[]=".txt";
    char first[1000],text_str[MAXCH], end[1000];
    char ch;
    int i=0;
    char str[50];
    char* token;
    char *commands[3];
    printf("Komut girin : \n");
    token = strtok(fgets(str, sizeof str, stdin), " ");
    int count = 0;
    while (token != NULL){
        commands[count] = token; 
        token = strtok(NULL, " ");
        count = count+1;
    }
    while(1){
      if((strcmp(commands[0],copy) == 0)){
        if(strcmp(commands[1],help) == 0 ){
          printf("Name\n\tcopy - copy file\nSynopsis\n\tcopy [oldfilename] [newfilename]\nDescription\n\tcopy - copies to another file\n");
        }
        else if (strcmp(commands[1],help) != 0){
          copy_file(commands[1],commands[2]);
        }
      }
      else if(strcmp(commands[0],create) == 0){
        if((strcmp(commands[1],help) == 0)){
          printf("Name\n\tcreate - create file\nSynopsis\n\tcreate [filename]\nDescription\n\tcreate - creates file\n");
        }
        else if (strcmp(commands[1],help) != 0){
          create_file(commands[1]);
        }
      }

      else if(strcmp(commands[0],move) == 0){
        if(strcmp(commands[1],help) == 0){
          printf("Name\n\tmove - move file\nSynopsis\n\tmove [filename] [folder]\nDescription\n\tmove - Moving file from one folder to another\n");
        }
        else if (strcmp(commands[1],help) != 0){
          move_file(commands[1],commands[2]);
        }
      }
      
      else if(strcmp(commands[0],addtext) == 0){
        if(strcmp(commands[1],help) == 0){
          printf("Name\n\tadd_text - add_text file\nSynopsis\n\tadd_text [filename]\nDescription\n\tadd_text - Adds text to the end of the file\n");
        }
        else if(strcmp(commands[1],help) != 0){
          add_text(commands[1]);
        }
      }

      else if(strcmp(commands[0],removeContent) == 0){
        if( strcmp(commands[1],help)== 0){
          printf("Name\n\tremove_content - remove_content file\nSynopsis\n\tremove_content [filename]\nDescription\n\tremove_content - Removes all text contained in a file\n");
      }
        else if(strcmp(commands[1],help) != 0){
          remove_content(commands[1]);
        }
      }
      
      else if((strcmp(commands[0],remove) == 0)){
        if((strcmp(commands[1],help) == 0)){
          printf("Name\n\tremove - remove file\nSynopsis\n\tremove [filename]\nDescription\n\tremove - deletes the file\n");
      }
        else if(strcmp(commands[1],help) != 0){
          remove_file(commands[1]);
        }
      }
    
      else if(strcmp(commands[0],rename) == 0){
        if(strcmp(commands[1],help) == 0){
          printf("Name\n\trename - rename file\nSynopsis\n\trename [oldfilename] [newfilename]\nDescription\n\trename- renames the file\n");
        }
        else if(strcmp(commands[1],help) != 0){
          rename_file(commands[1], commands[2]);
        }
      }
      else if(strcmp(commands[0],insert) == 0){
        if(strcmp(commands[1],help) == 0){
          printf("Name\n\tinsert - insert file\nSynopsis\n\tinsert [filename] [index]\nDescription\n\tinsert - Adds text to index specified in file.\n");
        }
        else if(strcmp(commands[1],help) != 0){
          insert_text(commands[1],atoi(commands[2]));
        }
      }
      else {
          printf("Geçersiz komut.\n");
          exit(0);
      }

      printf("Komut girin : \n");
      token = strtok(fgets(str, sizeof str, stdin), " ");
      int count = 0;
      while (token != NULL){
          commands[count] = token; 
          token = strtok(NULL, " ");
          count = count+1;
      }
    }
    free(create);
    free(remove);
    free(rename);
    free(copy);
    free(move);
    free(addtext);
    free(removeContent);
    free(insert);
    free(help);
    free(token);
    return 0;
}