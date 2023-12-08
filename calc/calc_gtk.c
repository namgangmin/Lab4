
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
typedef struct _Data Data;

struct _Data {
GtkWidget *Window;
GtkWidget *BtnAdd;
GtkWidget *BtnSub;
GtkWidget *BtnMul;
GtkWidget *BtnDiv;
GtkWidget *BtnResult;
GtkWidget *BtnReset;
GtkWidget *ResultEntry;
float num1; 
float num2; 
const gchar *cleartext;  
int isAdd;
int isSub;
int isMul;
int isDiv;
};
G_MODULE_EXPORT void quit (GtkWidget *window,gpointer data){
	gtk_main_quit ();
}
 
G_MODULE_EXPORT void on_AddBtn_clicked(GtkButton *button, Data *data){
  if(data->num2==-1){ 

    const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
    char* result_stringadd = malloc(sizeof(char) * 10);
    memset(result_stringadd, 0, 10);
    data->num1=atof(entrytemp); 
 
    sprintf(result_stringadd,"",entrytemp);

    gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringadd);
    data->isAdd=1;
    data->isSub=0;
    data->isMul=0;
    data->isDiv=0;
  }
}
 
G_MODULE_EXPORT void on_SubBtn_clicked(GtkButton *button, Data *data){
  if(data->num1==-1){ 
    const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
    char* result_stringsub = malloc(sizeof(char) * 10);
    memset(result_stringsub, 0, 10);
    data->num1=atof(entrytemp);
 
    sprintf(result_stringsub,"",entrytemp);
    gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringsub);
    data->isAdd=0;
    data->isSub=1;
    data->isMul=0;
    data->isDiv=0;
  }
}
 
G_MODULE_EXPORT void on_MulBtn_clicked(GtkButton *button, Data *data){
  if(data->num1==-1){ 
 const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
    char* result_stringmul = malloc(sizeof(char) * 10);
    memset(result_stringmul, 0, 10);
    data->num1=atof(entrytemp); 
    sprintf(result_stringmul,"",entrytemp);
    gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringmul); 
    data->isAdd=0;
    data->isSub=0;
    data->isMul=1;
    data->isDiv=0;
  }
}
 
G_MODULE_EXPORT void on_DivBtn_clicked(GtkButton *button, Data *data){
  if(data->num1==-1){ 
    const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
    char* result_stringdiv = malloc(sizeof(char) * 10);
   memset(result_stringdiv, 0, 10);
   data->num1=atof(entrytemp);  
    sprintf(result_stringdiv,"",entrytemp);
    gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringdiv); 
   data->isAdd=0;
    data->isSub=0;
    data->isMul=0;
    data->isDiv=1;
  }
}
G_MODULE_EXPORT void on_ClearBtn_clicked(GtkButton *button, Data *data){
  initData(data);
}
 
G_MODULE_EXPORT void on_ResultBtn_clicked(GtkButton *button, Data *data){
    char* result_stringres = malloc(sizeof(char) * 10);
    memset(result_stringres, 0,10);
 
   if(data->num1 != -1){ 
      const char *entrytemp=gtk_entry_get_text(GTK_ENTRY(data->ResultEntry));
  
      data->num2=atof(entrytemp); 
           
      if(data->isAdd==1){ 
       sprintf(result_stringres,"%.1f+%.1f=%.1f",data->num1,data->num2,data->num1+data->num2);
       gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringres); 
      }
      else if(data->isSub==1){ 
       sprintf(result_stringres,"%.1f-%.1f=%.1f",data->num1,data->num2,data->num1-data->num2);
       gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringres); 
      }
      else if(data->isMul==1){        
      sprintf(result_stringres,"%.1f*%.1f=%.1f",data->num1,data->num2,data->num1*data->num2);
       gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringres);
      }
      else if(data->isDiv==1){
       sprintf(result_stringres,"%.1f/%.1f=%.1f",data->num1,data->num2,data->num1/data->num2);
       gtk_entry_set_text(GTK_ENTRY(data->ResultEntry),result_stringres);
      }
      data->isAdd=0;
      data->isSub=0;
      data->isMul=0;
      data->isDiv=0;
  }
}
 
void initData(Data *data){

 char* result_string = malloc(sizeof(char) * 10);

 memset(result_string, 0, 10);
 result_string="";*/
 data->num1=-1;
 data->num2=-1;
 data->isAdd=0;
 data->isSub=0;
 data->isMul=0;
 data->isDiv=0;
 gtk_entry_set_text(GTK_ENTRY(data->ResultEntry)," ");  
}
 
 
int main (int argc, char *argv[]){
 GtkBuilder *builder;
 GError *error;
 Data *data;
 gtk_init (&argc, &argv);

 builder = gtk_builder_new ();
 gtk_builder_add_from_file (builder,"~/LAB4/calc/calcView.glade", NULL);      

 data = g_slice_new (Data);

 data->Window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
 data->BtnAdd = GTK_WIDGET(gtk_builder_get_object(builder, "AddBtn"));
 data->BtnSub = GTK_WIDGET(gtk_builder_get_object(builder, "SubBtn"));
 data->BtnMul = GTK_WIDGET(gtk_builder_get_object(builder, "MulBtn"));
 data->BtnDiv = GTK_WIDGET(gtk_builder_get_object(builder, "DivBtn"));
 
 
 data->BtnResult = GTK_WIDGET(gtk_builder_get_object(builder, "ResultBtn"));
 data->BtnReset = GTK_WIDGET(gtk_builder_get_object(builder, "ClearBtn"));
 
 data->ResultEntry = GTK_WIDGET(gtk_builder_get_object (builder, "ResultEntry"));
 
 gtk_builder_connect_signals (builder, data);
 
 g_object_unref (G_OBJECT (builder));
 
 gtk_widget_show_all (data->Window);
 
 initData(data);
 gtk_main ();
 g_slice_free (Data, data);
return (0);
}
