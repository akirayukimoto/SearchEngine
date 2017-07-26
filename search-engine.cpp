
#include <string.h>
#include "search-engine.h"
//#include "webcrawl.h"

//char *nextWord(char *p);

DictionaryType dictType;

SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
  // Create dictionary of the indicated type
  //
	FILE *note = fopen("not.txt", "a");


	if (dictionaryType == ArrayDictionaryType)
		_wordToURLList = new ArrayDictionary();
	else if (dictionaryType == BinarySearchDictionaryType)
		_wordToURLList = new BinarySearchDictionary();
	else if (dictionaryType == HashDictionaryType)
		_wordToURLList = new HashDictionary();
	else if (dictionaryType == AVLDictionaryType)
		_wordToURLList = new AVLDictionary();
	else NULL;
  // Populate dictionary and sort it if necessary
	fprintf(note, "%s\n", "Initializing variables");
	dictType = dictionaryType;
	int count = 0;
	int countLine = 0;
	char c;

	char *temp = new char[1000];
	//char *element;
	//fprintf(note, "%s\n", "url.txt");
	FILE *f1 = fopen("url.txt", "r");
	if (f1 == NULL) {
		exit(1);
	}

	URLRecord **list = new URLRecord*[1024];
	for (int i = 0; i < 1024; i++) {
		list[i] = new URLRecord();
	}
	while (fgets(temp, 1000, f1)) {
		if (strcmp(temp, "\n") != 0) {
			char *token = new char[1000];
			token = strtok(temp, " \n");
			int index = atoi(token);
			
			token = strtok(NULL, " \n");			
			char *link = new char[1000];
			strcpy(link, token);

			fgets(temp, 1000, f1);

			char *desc = new char[1000];
			token = strtok(temp, "\n");
			strcpy(desc, token);

			list[index]->_url = strdup(link);
			list[index]->_description = strdup(desc);
		}
		

	}
	delete temp;
	//fprintf(note, "%s\n", "End of url.txt");
	fclose(f1);
	

	f1 = fopen("word.txt", "r");

	if (f1 == NULL) exit(1);

	temp = new char[1000];

	while (fgets(temp, 1000, f1)) {
		if (strcmp(temp, "\n") != 0) {
			char *token = new char[1000];
			token = strtok(temp, " \n");
			char *word = new char[1000];
			strcpy(word, token);
			URLRecordList *head = NULL;
			URLRecordList *prev = NULL;

			token = strtok(NULL, " \n");

			while (token != NULL) {
				int index = atoi(token);
				if (list[index]->_url == NULL) continue;

					URLRecordList *curr = new URLRecordList();
					if (head == NULL) 
						head = curr;

					curr->_urlRecord = list[index];
					curr->_next = NULL;

					if (prev != NULL) 
						prev->_next = curr;

					prev = curr;
	
					token = strtok(NULL, " \n");
				
			}
			_wordToURLList->addRecord(word, (URLRecordList *)head);
			delete word;
			delete token;
		}
	}
	delete temp;
	fclose(f1);
	
	//fprintf(note, "%s\n", "END");
	//fclose(note);


}

bool
SearchEngine::match(char *&com) {
	if (strncmp(com, "/search?word=", 13) == 0) {
		com += 13;
		return true;
	}
	return false;
}

char *
SearchEngine::nextWord(char *&p) {
	char *word = (char *)malloc(sizeof(char) * 100);

	int i = 0;
	while (*p != '\0') {
		//if (*p != '\n' && *p != ' ' && *p != '+') {
		//	word[i] = *p;
		//	i++;
		//}
		//else {
		if (*p == '\n' || *p == ' ' || *p == '+') {
			if (i == 0) {
				p++;
				continue;
			}
			else {
				word[i] = '\0';
				return word;
			}
			//p++;
		}
		else {
			word[i] = *p;
			i++;
		}
		
		p++;
	}
	if (i > 0) {
		word[i] = '\0';
		//i = 0;
		return word;
	}
	return NULL;
}


void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
  //FILE *note = fopen("open.txt", "a");

  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    //FILE *note = fopen("note.txt", "a");
    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for al the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired
 
 	if (strlen(documentRequested) <= 13) return;

	char *temp = new char[strlen(documentRequested)];
	strcpy(temp, documentRequested + 13);
	printf("%s\n", temp);
	//char *p = temp;
	
	char **wordList = new char*[100];
	for (int i = 0; i < 100; i++) 
		wordList[i] = NULL;

	int index = 0;
//	char *tst = (char *)malloc(50 * sizeof(char));
//	if (match(temp)) {
//		while ((tst = nextWord(temp)) != NULL) {
//			wordList[index++] = strdup(tst);
//			printf("%d %s\n", index, tst);
//		}
//	}
	char *token = strtok(temp, "+");
	while (token != NULL) {
		//wordList[index] = strdup(token);
//		//fprintf(note, "%s\n", wordList[index]);
		wordList[index] = new char[50];
		strcpy(wordList[index], token);
		index++;
		token = strtok(NULL, "+");
	}
	char *result = new char[1000];
	strcpy(result, "");
	for (int i = 0; i < index; i++) {
		//strcat(result, ",");
		strcat(result, wordList[i]);
		if (i != index - 1) strcat(result, " ");
		
	}

	printf("Words to search for: %s\n", result);

/**
  const int nurls=2;

  const char * words = "data structures";

  const char * urls[] = {
    "http://www.cs.purdue.edu",
    "http://www.cs.purdue.edu/homes/cs251"
  };
  
  const char * description[] = {
    "Computer Science Department. Purdue University.",
    "CS251 Data Structures"
  };
  */

  fprintf( stderr, "Search for words: \"%s\"\n", result);

  fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
  fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
  fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", result);

	int count = 0;
	int counter = 0;
	URLRecord **llist = new URLRecord*[1000];
	for (int i = 0; i < index; i++) {
		//fprintf(note, "%s\n", "find word");
		URLRecordList *e;
		e = (URLRecordList *)_wordToURLList->findRecord(wordList[i]);
		while (e != NULL) {
			int flag = 0;
			for (int j = 0; j < count; j++) {
				if (llist[i] == e->_urlRecord) {
					flag = 1;
					break;
					//fprintf(note, "Flag is 1\n");
				}
			}
			if (flag == 0) {
				llist[count] = e->_urlRecord;
				count++;
			}
			e = e->_next;
		}
	}
	for (int i = 0; i < count; i++) {
		int j;
		for (j = 0; j < index; j++) {
			URLRecordList *curr;
			curr = (URLRecordList *)_wordToURLList->findRecord(wordList[j]);
			int flag = 0;
			while (curr != NULL) {
				if (curr->_urlRecord == llist[i]) {
					flag = 1;
				}
				curr = curr->_next;
			}
			if (flag == 0) 
				llist[i] == NULL;
		}
	}


  for ( int i = 0; i < count; i++ ) {
  	if (llist[i] == NULL) continue;
    	fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", counter + 1, llist[i]->_url, llist[i]->_url);
    	fprintf( fout, "<blockquote>%s<p></blockquote>\n", llist[i]->_description);
    	counter++;
	
  }

  // Add search form at the end

  //fclose(note);
  //
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
}





void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  // Get DictionaryType
  const char * dictType = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(dictType, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(dictType, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(dictType, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(dictType, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }
  
  SearchEngine httpd(port, dictionaryType);
  
  httpd.run();

  return 0;
} 
