# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <string>
# include <cstring>
# include <iostream>
using namespace std;

# define NOT !
# define AND &&
# define OR ||
# define EQ ==

typedef char * CharPtr;
enum TokenType { IDENTIFIER = 345, CONSTANT = 678, SPECIAL = 248 } ;


struct Column {
	int column ;
	Column * next;
};

typedef Column * ColumnPtr;


struct Line {
	int line;
	ColumnPtr firstAppearAt;
	ColumnPtr lastAppearAt;
	Line * next;
};

typedef Line * LinePtr;

struct Token {
	CharPtr tokenStr;
	TokenType type;
	LinePtr firstAppearOn; 
	LinePtr lastAppearOn ;
	Token * next;
};

typedef Token * TokenPtr;

static int uTestNum = -1;
int gNumOfInput = -1;

int gLine = 1 ;
int gColumn = 1 ;
int gcount = 0 ;

char gNextChar = '\0';
int gNextCharLine = -1;
int gNextCharColumn = -1;
TokenPtr gpreToken = NULL ;

void GetNextChar( char & ch, int & line, int & column );
bool IsWhiteSpace( char ch );
bool IsDigit( char ch );
bool IsLetter( char ch );
bool IsSpecial( char ch );

void SkipWhiteSpaces();
void SkipLineComment();
void SkipMultiLineComment();
CharPtr GetID();
CharPtr GetNum();
CharPtr GetCharConst();
CharPtr GetStringConst();
CharPtr GetSpecial();

void OrderInsertToken( TokenPtr & head, CharPtr tokenStr, TokenType tokenType, int tokenLine, int tokenColumn );
void SetToken( TokenPtr & head, CharPtr tokenStr, TokenType tokenType, int tokenLine, int tokenColumn ) ;
void SetLineCol( TokenPtr & head, int tokenLine, int tokenColumn ) ;

void CopyStringToChar( CharPtr &str, string tmp );
bool HasToken( TokenPtr head, CharPtr tokenStr ) ;

CharPtr GetToken( TokenType & tokenType, int & firstCharLine, int & firstCharColumn );
void PrintAllTokenInfo( int max, TokenPtr head );
int Size( CharPtr token ) ; 
void SortList( TokenPtr & haed ) ;
void Sort( TokenPtr & current, TokenPtr head );
void NewlyInsert( TokenPtr current, TokenPtr newToken );
void InsetToEnd( TokenPtr & head, TokenPtr newToken ) ;
TokenPtr FindPos( TokenPtr head, CharPtr tokenStr ) ;

int CountToken( TokenPtr head ) {
	
	if ( head == NULL )
	  return 0;
	
	int count = 0;
	while ( head != NULL ) {
		count++ ;
		head = head->next ;
	} // while
	return count ;
} // count token

int CountIden( TokenPtr head ) {
	
	if ( head == NULL )
		return 0;
		
	int count = 0 ;
	while ( head != NULL ) {
		if ( head->type == IDENTIFIER )
		 count++ ;
		head = head->next ;
	} // while
	return count ;
}  // count identifer

int CountConst( TokenPtr head ) {
	
	if ( head == NULL )
		return 0 ;
		
	int count = 0 ;
	while ( head != NULL ) {
		if ( head->type == CONSTANT ) {
			count++ ;
		} // if
		head = head->next ;
	} // while
	return count ;
} // count const

int CountSpecial( TokenPtr head ) {
	
	if ( head == NULL )
		return 0;
		
	int count = 0 ;
	while ( head != NULL ) {
		if ( head->type == SPECIAL )
			count++;
		head = head->next ;
	} // while
	return count ;
} // CountSpecial

void PrintThisTokenLine( TokenPtr head ) {
	// for command 3
	
	LinePtr lineWalk = NULL ; //head->firstAppearOn ;
	ColumnPtr colWalk = NULL ;
	
	for ( lineWalk = head->firstAppearOn ; lineWalk != NULL ; lineWalk = lineWalk->next ) {
		for ( colWalk = lineWalk->firstAppearAt ; colWalk != NULL ; colWalk = colWalk->next ) {
			printf( "(%d,%d)", lineWalk->line, colWalk->column ) ;
			
			if ( colWalk->next != NULL )
				cout << " " ;
		} // for
		
		if ( lineWalk->next != NULL )
			cout << " ";
	} // for
	
} // print this token line info

void OnThisLine( TokenPtr head, int line ) {
	
	/*if ( head == NULL ) 
		return ;*/
	
	while ( head != NULL ) {
		LinePtr runner = head->firstAppearOn ;
		int count = 0 ;
		
		while ( runner != NULL && count == 0 ) {
			
			if ( runner->line == line ) {
				printf( "%s\n", head->tokenStr ) ;
				count++ ;
			} // if
			
			runner = runner->next ;
		} // while
		
		head = head->next ;
	} // while
	
	/*if ( !print ) {
		printf("\n") ;
	} // if */
	
} // on this line

void HandleUI( TokenPtr head ) {
	string command ;
	printf( "\nWelcome to interactibe I/O.\n" ) ;
	printf( "1. How many different tokens are there?\n" ) ;
	printf( "2. How many tokens are there for each case?\n" ) ;
	printf( "3. Print (line number, column number) of this token ->\n" ) ;
	printf( "4. What tokens appear on this line ->\n" ) ;
	printf( "5. End.\n" ) ;
	printf( "\n" ) ;
	printf( "> " ) ;
	cin >> command ;
	
	while ( strcmp( command.c_str(), "5" ) != 0 ) {
		
		if ( strcmp( command.c_str(), "1" ) == 0 ) {
			//printf( "> " ) ;
			int count = CountToken( head ) ;
			printf( "There are %d different tokens in total.\n", count ) ;
		} // if
		else if ( strcmp( command.c_str(), "2" ) == 0 ) {
			//printf( "> " ) ;
			int count1 = CountIden( head ) ;
			int count2 = CountConst( head ) ;
			int count3 = CountSpecial( head ) ;
			printf( "Identifer : %d\n", count1 ) ;
			printf( "Identifer : %d\n", count2 ) ;
			printf( "Special : %d\n", count3 ) ;
		} // else if 
		else if ( strcmp( command.c_str(), "3" ) == 0 ) {
			
			string str ;
			cin >> str ;
			//printf( "> " ) ;
			str += '\0' ;
			CharPtr tokenStr = ( char* ) malloc( str.size() ) ;
			CopyStringToChar( tokenStr, str ) ;
			bool hasToken = HasToken( head, tokenStr ) ;
			if ( hasToken ) {
				TokenPtr tmp = FindPos( head, tokenStr ) ;
				printf( "%s ", tokenStr ) ;
				PrintThisTokenLine( tmp ) ;
				printf( "\n" ) ;
			} // if
			else {
				printf( "There is no such token : %s\n", tokenStr ) ;
			} // else
		} // else if
		else if ( strcmp( command.c_str(), "4" ) == 0 ) {
			
			string line ;
			cin >> line ;
			int lineNum = atoi( line.c_str() ) ;
			//printf( "> " ) ;
			OnThisLine( head, lineNum ) ;
		} // else if
		else {
			printf( "Invalid optione: %s\n", command.c_str() ) ;
		} // else 
		
		
		cout << endl ;
		cout << "> " ;
		cin >> command ;

	} // while
	
} // handle ui


int main() {
	char ch = '\0';
	
	CharPtr tokenStr = NULL;
	int tokenLine = 0, tokenColumn = 0;
	TokenPtr listHead = NULL ;
	TokenType tokenType ;
	
	scanf( "%d%c", &uTestNum, &ch );
	GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	
	do {
		
		tokenStr = GetToken( tokenType, tokenLine, tokenColumn );
		
		if ( strcmp( tokenStr, "END_OF_FILE" ) != 0 )
			OrderInsertToken( listHead, tokenStr, tokenType, tokenLine, tokenColumn ) ;
		

	} while ( strcmp( tokenStr, "END_OF_FILE" ) != 0 ) ;
	
	if ( uTestNum < 3 ) {
		PrintAllTokenInfo( 1000, listHead ) ;
	} // if
	else if ( uTestNum == 3 )
		PrintAllTokenInfo( 55, listHead ) ;
	else if ( uTestNum == 4 )
		PrintAllTokenInfo( 18, listHead ) ;
	else if ( uTestNum == 5 )
		PrintAllTokenInfo( 1000, listHead ) ;
	else
		printf( "no\n" ) ; 
	
	HandleUI( listHead ) ;
	return 0;
} // main()

TokenPtr AgainPos( TokenPtr head, CharPtr tokenStr ) {
	
	while ( strcmp( head->tokenStr, tokenStr ) != 0 ) {
		head = head->next ;	
	} // while
	
	return head ;
} // find the pos if the tokenStr has exist in token list

void PrintAllTokenInfo( int max, TokenPtr token ) {
	/*
	while ( token != NULL ) {
		printf( "%s ", token->tokenStr ) ;
		while ( token->firstAppearOn != NULL ) {
			printf( "(%d,", token->firstAppearOn->line ) ;
			printf( "%d) ", token->firstAppearOn->firstAppearAt->column ) ;
			token->firstAppearOn = token->firstAppearOn->next ;
		} // while
		cout << endl ;
		token = token->next ;
	} // while
	*/
	int count = 0;
	while ( token != NULL ) {
		printf( "%s ", token->tokenStr );
		LinePtr lineWalk = NULL;
		ColumnPtr columnWalk = NULL;
		
		for ( lineWalk = token->firstAppearOn ; lineWalk != NULL ; lineWalk = lineWalk->next ) {
			for ( columnWalk = lineWalk->firstAppearAt ; columnWalk != NULL ; columnWalk = columnWalk->next ) {
				printf( "(%d,%d)", lineWalk->line, columnWalk->column );
			} // for
		} // for
		
		printf( "\n" );
		count++ ;
		token = token->next;
	} // while()
} // print all token informatiom

void InsertToEnd( TokenPtr & head, TokenPtr newToken ) {
	if ( head == NULL ) {
		head = newToken ;
		return ;
	} // if
	else {
		return InsertToEnd( head->next, newToken );
	} // else
} // InsertToEnd()

void OrderInsertToken( TokenPtr & head, CharPtr tokenStr, TokenType tokenType, int tokenLine, int tokenColumn ) {	
	if ( head == NULL ) {
		TokenPtr newToken = new Token ;
		SetToken( newToken, tokenStr, tokenType, tokenLine, tokenColumn ) ;
		head = newToken ;
		return ; 
	} // if	
	
	if ( !HasToken( head, tokenStr ) ) {
		// insert
		TokenPtr newToken = new Token ;
		SetToken( newToken, tokenStr, tokenType, tokenLine, tokenColumn ) ;
		InsertToEnd( head, newToken ) ;
	} // else if
	else {
		TokenPtr pos = FindPos( head, tokenStr ) ;
		SetLineCol( pos , tokenLine, tokenColumn ) ;
		return ;
	} // else

	
	Sort( head, head );

	return ;
} // order insert token


TokenPtr FindPos( TokenPtr head, CharPtr tokenStr ) {
	if ( head == NULL )
		return NULL ;
	else if ( strcmp( head->tokenStr, tokenStr ) == 0 )
	  return head ;
	else {
		return FindPos( head->next, tokenStr ) ;
	} // else
	
} // find pos

void NewlyInsert( TokenPtr current, TokenPtr newToken ) {
	if ( current == NULL ) {
		current = new Token;
		current = newToken;
	} // if
	else {
		current = current->next;
		NewlyInsert( current, newToken );
	} // else
	
} // NewlyInsert()

void Sort( TokenPtr & current, TokenPtr head ) {
	for ( TokenPtr tokenWalk = current ; tokenWalk != NULL ; tokenWalk = tokenWalk->next ) {
		for ( TokenPtr nextWalk = current ; nextWalk->next != NULL ; nextWalk = nextWalk->next ) {
			
			if ( strcmp( tokenWalk->tokenStr, nextWalk->tokenStr ) < 0 ) {
				// swap two pointers element

				TokenPtr temp = new Token;
				temp->tokenStr = tokenWalk->tokenStr;
				temp->type = tokenWalk->type;
				temp->firstAppearOn = tokenWalk->firstAppearOn;
				temp->lastAppearOn = tokenWalk->lastAppearOn;
				
				tokenWalk->tokenStr = nextWalk->tokenStr;
				tokenWalk->type = nextWalk->type;
				tokenWalk->firstAppearOn = nextWalk->firstAppearOn;
				tokenWalk->lastAppearOn = nextWalk->lastAppearOn;
							
				nextWalk->tokenStr = temp->tokenStr;
				nextWalk->type = temp->type;
				nextWalk->firstAppearOn = temp->firstAppearOn;
				nextWalk->lastAppearOn = temp->lastAppearOn;
				
			} // if
		} // for
	} // for
	
} // Sort()



bool HasToken( TokenPtr head, CharPtr tokenStr ) {
	if ( head == NULL )
		return false ;
	
	if ( strcmp( head->tokenStr, tokenStr ) == 0 )
		return true;
	else
		return HasToken( head->next, tokenStr ) ;
	
	return false;
	
} // check again



int Size( CharPtr token ) {
	//int count = 0 ;
	int i = 0 ;
	while ( token[i] != '\0' ) {
		//count++;
		i++;
	} // while
	
	return i ;
} // size()


void SetLineCol( TokenPtr & head, int tokenLine, int tokenColumn ) {
	
	// if the tokenLine equal the specific token pointer first line 
	if ( tokenLine == head->firstAppearOn->line ) {
		ColumnPtr tmp = head->firstAppearOn->firstAppearAt ;
		//printf( "Cortex: %s: ( %d, %d )\n", head->tokenStr, tokenLine, tokenColumn );
		while ( tmp->next != NULL ) {
			// find where to be inserted
			tmp = tmp->next ;
		} // while
		
		ColumnPtr newCol = new Column ;
		newCol->column = tokenColumn ;
		newCol->next = NULL ;
		tmp->next = newCol ;
		head->firstAppearOn->lastAppearAt = newCol ; // update the last appear at 
		return ; 
	} // if
	else if ( tokenLine > head->firstAppearOn->line ) {
		LinePtr tmp = head->firstAppearOn ;
		
		while ( tmp->next != NULL ) {
			// find where to be inserted
			tmp = tmp->next ;
		} // while
		
		LinePtr newLine = new Line ;
		newLine->line = tokenLine ;
		
		newLine->firstAppearAt = new Column ;
		newLine->lastAppearAt = new Column ;
		
		newLine->firstAppearAt->column = tokenColumn ;
		newLine->firstAppearAt->next = NULL ;
		
		newLine->lastAppearAt->column = tokenColumn ;
		newLine->lastAppearAt->next = NULL ;
		
		tmp->next = newLine ;
		tmp->next->next = NULL ;
		head->lastAppearOn = newLine ; // update the line number
		return ;
	} // else if
	
} // set line and col

void SetToken( TokenPtr & head, CharPtr tokenStr, TokenType tokenType, int tokenLine, int tokenColumn ) {
	
    head->tokenStr = tokenStr ;
    head->type = tokenType ;
    
    head->firstAppearOn = new Line ;
    head->lastAppearOn = new Line ;
    
    
    // initialize the head->firstAppearOn
    head->firstAppearOn->line = tokenLine ;
    head->firstAppearOn->firstAppearAt = new Column ;
    head->firstAppearOn->lastAppearAt = new Column ;
    
	head->firstAppearOn->firstAppearAt->column = tokenColumn ;
	head->firstAppearOn->lastAppearAt->column = tokenColumn ;
	
	head->firstAppearOn->firstAppearAt->next = NULL ;
	head->firstAppearOn->lastAppearAt->next = NULL ;
	head->firstAppearOn->next = NULL ;
	
	
	//initialize lastAppearOn
    head->lastAppearOn->line = tokenLine ;
    head->lastAppearOn->firstAppearAt = new Column ;
    head->lastAppearOn->lastAppearAt = new Column ;
    
    head->lastAppearOn->firstAppearAt->column = tokenColumn ;
    head->lastAppearOn->lastAppearAt->column = tokenColumn ;
    
    head->lastAppearOn->firstAppearAt->next = NULL ;
    head->lastAppearOn->lastAppearAt->next = NULL ;
    head->lastAppearOn->next = NULL ;
	
	
	head->next = NULL ; 
} // set token


void PrintTokenInfo( CharPtr tokenStr, TokenType tokenType, int tokenLine, int tokenColumn ) {
	
	printf( "%s ", tokenStr );
	
	if ( tokenType == IDENTIFIER )
		printf( "IDENT" );
	else if ( tokenType == CONSTANT )
		printf( "CONSTANT" );
	else if ( tokenType == SPECIAL )
		printf( "SPECIAL" );
		
	printf( " (%d, %d)\n", tokenLine, tokenColumn );
} // print token info


CharPtr GetToken( TokenType & tokenType, int & firstCharLine, int & firstCharColumn ) {
	
	CharPtr str = ( char* )malloc( sizeof(char) * 30 );
	bool slashIsDIVIDE = false;
	int lineOfDIVIDE = -1, columnOfDIVIDE = -1;
	char tmp;
	while ( IsWhiteSpace( gNextChar ) || ( gNextChar == '/' && !slashIsDIVIDE ) ) {
		//deal with whitespace and comment
		
		if ( IsWhiteSpace( gNextChar ) )
			SkipWhiteSpaces();
		
		else if ( gNextChar == '/' ) {
			lineOfDIVIDE = gNextCharLine, columnOfDIVIDE = gNextCharColumn;
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
			
			if ( gNextChar == '/' )
				SkipLineComment();
			else if ( gNextChar == '*' )
				SkipMultiLineComment();
			else
				slashIsDIVIDE = true;
		} // else if
		
	} // while
	
	int line = gNextCharLine, column = gNextCharColumn;
	
	if ( slashIsDIVIDE ) {
		if ( gNextChar == '=' ) {
			str[0] = '/';
			tokenType = SPECIAL;
			firstCharLine = gNextCharLine, firstCharColumn = gNextCharColumn;
			firstCharColumn--;
			str[1] = '=';
			str[2] = '\0';
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		} // if
		else {
			tokenType = SPECIAL;
			str[0] = '/';
			str[1] = '\0';
			firstCharLine = lineOfDIVIDE, firstCharColumn = columnOfDIVIDE;
		} // else
	} // if
	else if ( IsLetter( gNextChar ) || gNextChar == '_' ) {
		tokenType = IDENTIFIER;
		firstCharLine = gNextCharLine, firstCharColumn = gNextCharColumn;
		str = GetID() ;
	} // else if
	else if ( IsDigit( gNextChar ) || gNextChar == '.' ) {
		tokenType = CONSTANT;
		firstCharLine = gNextCharLine, firstCharColumn = gNextCharColumn;
		str = GetNum();
	} // else if digit
	else if ( gNextChar == '\'' ) {
		tokenType = CONSTANT;
		firstCharLine = gNextCharLine, firstCharColumn = gNextCharColumn;
		str = GetCharConst();
	} // else if '
	else if ( gNextChar == '\"' ) {
		tokenType = CONSTANT;
		firstCharLine = gNextCharLine, firstCharColumn = gNextCharColumn;
		str = GetStringConst();
	} // else if
	else if ( IsSpecial( gNextChar ) ) {
		tokenType = SPECIAL;
		firstCharLine = gNextCharLine, firstCharColumn = gNextCharColumn;
		str = GetSpecial();
	} // else if

	
    //printf("%s\n",str);
	return str;
} // get token


CharPtr GetID() {
	
	string store = "" ;
	char tmp = gNextChar;
	store += tmp;
	GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	
	while ( IsLetter( gNextChar ) || gNextChar == '_' || IsDigit( gNextChar ) ) {
		store += gNextChar ;
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	} // while
	
	store += '\0';
	CharPtr id = ( char *)malloc( store.size() );
	CopyStringToChar( id, store );
	return id;
} // GetID()

CharPtr GetNum() {

	string store = "" ;
	store += gNextChar;
	int hasDot = 0;
	if ( gNextChar == '.' )
		hasDot++;
	GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	if ( gNextChar == '.' )
		hasDot++;
		
	while ( ( IsDigit( gNextChar ) || gNextChar == '.' ) && hasDot <= 1 ) {
		store += gNextChar;
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		if ( gNextChar == '.' )
			hasDot++;
	} // while
	
	store += '\0';
	CharPtr num = ( char *)malloc( store.size() );
	CopyStringToChar( num, store );
	return num;
} // GetNum()

CharPtr GetCharConst() {
	string store = "";
	store += gNextChar;
	GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	store += gNextChar;
	
	if ( gNextChar == '\\' ) {
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		
		if ( gNextChar == 'n' || gNextChar == 't' || gNextChar == '0' ) {
			store += gNextChar;
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
			store += gNextChar;
		} // if
		else {
			store += gNextChar;
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
			store += gNextChar;
		} // else
	} // if
	else {
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		store += gNextChar;
	} // else
	
	store += '\0';
	CharPtr ch = ( char *)malloc( store.size() );
	CopyStringToChar( ch, store );
	GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	return ch; 
} // GetCharConst()

CharPtr GetStringConst() {
	
	string store = "" ;
	store += gNextChar;
	GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	
	while ( gNextChar != '\"' ) {
		if ( gNextChar == '\\' ) {
			char prev = gNextChar;
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
			if ( gNextChar == 'n' || gNextChar == 't' || gNextChar == '0' || gNextChar == '\"' ) {
				store += prev;
			} // if
			
			store += gNextChar;
		} // if
		else {
			store += gNextChar;
		} // else
		
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	} // while
	
	store += gNextChar;
	store += '\0';
	CharPtr str = ( char* )malloc( store.size() ) ;
	CopyStringToChar( str, store );
	GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
	return str;
	
} // GetStringConst()

void CopyStringToChar( CharPtr &str, string tmp ) {
	for ( int i = 0 ; i < tmp.size() ; i++ ) {
		str[i] = tmp[i];
	} // for
} // CopyStringToChar()

CharPtr GetSpecial() {
	string store = "";
	store += gNextChar;
	char prev = gNextChar;
		if ( gNextChar == '+' || gNextChar == '-' || gNextChar == '*' || gNextChar == '/' ||
	     gNextChar == '>' || gNextChar == '<' || gNextChar == '=' || gNextChar == '!' || gNextChar == '%' ) {
	    // deal with += case
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		if ( gNextChar == '=' ) {
			store += gNextChar;
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		} // if
		else if ( ( prev == '+' && gNextChar == '+' ) || ( prev == '-' && gNextChar == '-' ) ||
				  ( prev == '>' && gNextChar == '>' ) || ( prev == '<' && gNextChar == '<' ) || 
				  ( prev == '-' && gNextChar == '>' ) ) {
			// deal with double sign ex. ++
			store += gNextChar;
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		} // else if
	} // if
	else {
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		if ( ( prev == '&' && gNextChar == '&' ) || ( prev == '|' && gNextChar == '|' ) ) {
			store += gNextChar;
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		} // if
		
	} // else
	 
	store += '\0';
	CharPtr special = ( char *)malloc( store.size() );
	CopyStringToChar( special, store );
	 return special;
	 
} // GetStringConst()

void GetNextChar( char & ch, int & line, int & column ) {
	
	
	ch = getchar();
	line = gLine;
	column = gColumn;
	
	if ( ch == '\n' ) {
		gLine++;
		gColumn = 0;
	} // if
	
	gColumn++;
} // GetNextChar()

void SkipWhiteSpaces() {
	
	while ( IsWhiteSpace( gNextChar ) ) {
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		
		if ( !IsWhiteSpace( gNextChar ) ) 
		  return;
	} // while 
	
} // skip white

void SkipLineComment() {
	while ( gNextChar != '\n' ) {
		GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		
		if ( gNextChar == '\n' ) {
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
			return;
		} // if
	} // while
} // skip line comment

void SkipMultiLineComment() {
	bool eof = false;
	GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );

	while ( !eof ) {
		if ( gNextChar == '*' ) {
			char prev = gNextChar;
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
			if ( prev == '*' && gNextChar == '/' ) {
				eof = true;
				GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
				return ;
			} // if
		} // if
		else {
			GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
		} // else
	} // while
} // skip mutiple line cmment

bool IsLetter( char ch ) {
	
	if ( ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) ) {
		return true;
	} // if
	else
		return false;
	
} // is letter

bool IsDigit( char ch ) {
	
	if ( ch >= '0' && ch <= '9' )
	  return true;
	else
		return false;
} // is digit

bool IsWhiteSpace( char ch ) {
	
	if ( ch == ' ' || ch == '\n' || ch == '\t' )
		return true;
	else
		return false;
}  // is whitespace

bool IsSpecial( char ch ) {
	
	if ( ch == '^' || ch == ',' || ch == '(' || ch == ')' || ch == '[' || ch == ']' ||
	     ch == '{' || ch == '}' || ch == '#' || ch == '?' || ch == ':' || ch == ';' ||
		 ch == '!' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '>' ||
		 ch == '<' || ch == '=' || ch == '%' || ch == '&' || ch == '|' )
		return true;
	else
		return false;
		
} // is special
