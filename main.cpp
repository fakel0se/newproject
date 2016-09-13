/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ilya
 * Алалалала
 * Created on 27 июля 2016 г., 21:06
 */


#include <cstdlib>
#include <iostream> 
#include <string.h>
using namespace std;

/*
 * 
 */

    double expr();
    double term();
    double prim();
    double error(const char* s);
    
    enum token_value {
        NAME,      NUMBER,     END,
        PLUS='+',  MINUS='-',  MUL='*', DIV='/',
        PRINT=';', ASSIGN='=', LP='(', RP=')'
    };
    token_value curr_tok;
        
    token_value get_token();
    
    struct name {
        char* string;
        name* next;
        double value;
    };    
    
    name* look(const char* p, int ins);
    name* insert(const char* s);
    
    double number_value;
    char name_string[256];    
    const int TBLSZ = 23;
    name* table[TBLSZ];
    int no_of_errors;
    char* buf;
    
    double expr() {
        double left = term();
        while(1) {
            switch(curr_tok) {
                case PLUS: 
                    get_token();
                    left += term();
                    break;
                case MINUS:
                    get_token();
                    left -= term();
                    break;
                default:
                    return left;
            }
        }
    }    
 
    double term() {
        double left = prim();
        while(1) {
            switch(curr_tok) {
                case MUL: 
                    get_token();
                    left *= prim();
                    break;
                case DIV:
                {
                    get_token();
                    double d = prim();
                    if (d == 0) return error("division by zero"); 
                    left /= d;
                }    
                    break;
                default:
                    return left;
            }
        }   
    }
    
    name* look(const char* p, int ins =0) {
        int ii = 0;
        const char* pp = p;
        while (*pp) ii = ii<<1 ^ *pp++;
        if (ii < 0) ii = -ii;
        ii %= TBLSZ;
        for (name* n = table[ii]; n; n = n->next)
            if (strcmp(p, n->string) == 0) return n;
        if (ins == 0) error("name isn't be find");
        name* nn = new name;
        nn->string = new char[strlen(p) + 1];
        strcpy(nn->string,p);
        nn->value = 1;
        nn->next = table[ii];
        table[ii] = nn;
        return nn;
    }
    
    double prim() {
        switch (curr_tok) {
            case NUMBER:
                get_token();
                return number_value;
            case NAME:
                if (get_token() == ASSIGN) {
                    name* n = insert(name_string);
                    get_token();
                    n->value = expr();
                    return n->value;
                }
                return look(name_string)->value;
            case MINUS:    
                get_token();
                return -prim();
            case LP:    
            {
                get_token();
                double e = expr();
                if (curr_tok != RP) return error("need \')\'");
                get_token();
                return e;
            }    
            case END:
                return 1;
            default:
                return error("need primary");
        }
        
    }
    
    token_value get_token() {
        char ch;
	if (!buf) {
	    buf = new char[1024]; 
            delete[] buf;
            cin.getline(buf, 1024);	
	}
        if (!*buf) {
            delete[] buf;
	    buf = new char[1024]; 
            cin.getline(buf, 1024);            
        }

        do {
            ch = *buf++;
            if (!ch && !isalnum(ch)) return curr_tok = END;
        } while (ch != '\n' && isspace(ch));
        switch (ch) {
            case ';':    
            case '\n':
                //cin >> ws;
                return curr_tok = PRINT;            
            case '*':
            case '/':
            case '+':
            case '-':
            case '(':
            case ')':
            case '=':
                return curr_tok = token_value(ch);
            case '0': case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': case '9':  
            case '.':    
                buf--;
                number_value = atof(buf);
                int a;
                a = number_value;                
                while (a >= 1) {
                    a /= 10;
                    buf++;
                }
                return curr_tok = NUMBER;
            default:
                if (isalpha(ch)) {
                    char* p = name_string;
                    *p++ = ch;
                    while ((ch = *buf++) && isalnum(ch)) *p++ = ch;
                    buf--;
                    *p = 0;
                    return curr_tok = NAME;
                }
                error("incorrect lexem");
                return curr_tok = PRINT;
        }    
                    
    }
    
    
    inline name* insert(const char* s) { return look(s, 1);}    
    
    double error(const char* s) {
        cerr << "error: " << s << endl;
        no_of_errors++;
        return 1;
    }



/*
 *
 */

int main(int argc, char** argv) {
    cout << "Beginning" << endl;
    insert("pi")->value = 3.141592653;
    insert("e")->value = 2.71282818284;
    
    while (cin) {
        get_token();
        if (curr_tok == END) break;
        if (curr_tok == PRINT) continue;
        cout << expr() << endl;
    }

    
    return no_of_errors;
}



