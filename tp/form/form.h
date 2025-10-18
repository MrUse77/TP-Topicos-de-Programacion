#ifndef FORM_H_INCLUDED
#define FORM_H_INCLUDED

typedef struct Form_t *Form_t;
typedef struct Field_t *Field_t;

#define FORM_TITLE_LEN 64
#define FORM_FIELDS_AMOUNT 256

#define FIELD_LABEL_LEN 64
#define FIELD_ATTRIB_AMOUNT 8
#define FIELD_ATTRIB_VALUE_LEN 32

#define FIELD_TYPE_DEFAULT 4
#define FIELD_TYPE_RADIO 0
#define FIELD_TYPE_CHECKLIST 1
#define FIELD_TYPE_INT 2
#define FIELD_TYPE_NUMERIC 3
#define FIELD_TYPE_TEXT 4
#define FIELD_TYPE_DATE 5

#define FIELD_ATTRIB_MIN 0 // int, numeric, date
#define FIELD_ATTRIB_MAX 1 // int, numeric, date
#define FIELD_ATTRIB_MAX_LEN 2 // text
#define FIELD_ATTRIB_OPTIONS 3 // radio, checklist
#define FIELD_ATTRIB_HINT 4 // radio, checklist, int, numeric, text, date
#define FIELD_ATTRIB_MIN_CHECK 5 // checklist
#define FIELD_ATTRIB_MAX_CHECK 6 // checklist

/*
#define FIELD_ATTRIB_MIN "min" // int, numeric, date
#define FIELD_ATTRIB_MAX "max" // int, numeric, date
#define FIELD_ATTRIB_MAX_LEN "maxlen" // text
#define FIELD_ATTRIB_OPTIONS "options" // radio, checklist
#define FIELD_ATTRIB_HINT "hint" // radio, checklist, int, numeric, text, date
#define FIELD_ATTRIB_MIN_CHECK "maxcheck" // checklist
#define FIELD_ATTRIB_MAX_CHECK "mincheck" // checklist
*/


Form_t* formCreate(const char* title);

int formSetTitle(Form_t* form, const char* title);

char* formGetTitle(Form_t* form);

int formSetFields(Form_t* form, Field_t** fields);

Field_t* formGetFields(Form_t* form);

int formGetFieldAmount(Form_t* form);

int formPost(Form_t* form);

int formDestroy(Form_t* form);


Field_t* fieldCreate(const char* label);

int fieldSetType(Field_t* field, int inputType);

int fieldGetType(const Field_t* field);

int fieldSetAttrib(Field_t* field, int attribType, char* value);

char* fieldGetAttrib(const Field_t* field, int attribType);

int fieldPrintAttrib(const Field_t* field, int attribType);

char* fieldGetBuffer(const Field_t* field);

int fieldDestroy(Field_t* field);

void printAttribList(void);

#endif // FORM_H_INCLUDED