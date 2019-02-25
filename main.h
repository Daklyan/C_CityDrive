//
// Created by daklyan on 01/02/19.
//

typedef char *string;
void curlWrite(void);
void jsonParser(void);
string reader(FILE * ptr);
long sizer(FILE * ptr);
void doit(string text);
void parseObject(cJSON * object);
GdkPixbuf *createPixbuf(const gchar * logoname);
void buttonClicked(void);
void display(string text);

