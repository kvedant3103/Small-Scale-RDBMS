#include<bits/stdc++.h>
using namespace std;
#define MAX_LINE 2048

unordered_map<string, vector<string>> m;

/////////////////////////////////////////////////////  case insensitive string compare  ////////////////////////////////////////
bool compareChar(char & c1, char & c2)
{
    if (c1 == c2)
        return true;
    else if (toupper(c1) == toupper(c2))
        return true;
    return false;
}

bool caseInSensStringCompare(string str1, string str2)
{
    return ((str1.size() == str2.size() ) && equal(str1.begin(), str1.end(), str2.begin(), &compareChar));
}

//////////////////////////////////////////////  get line count  ///////////////////////////////////////////////////////////
int get_line_count(string t_name){
    string line;
    int c=0;
    ifstream input;
    input.open(t_name+".txt");

    while(getline(input, line)){
        c++;
    }
    input.close();
    return c;
}

////////////////////////////////////////////////////  populate_map  ///////////////////////////////////////////////
void populate_map(){
    fstream fin;
    fin.open("schema.txt", ios::in);

    string line;
    int flag = 0;

    while (getline(fin, line))
    {
        stringstream word(line);
        vector<string> v;
        while (word.good())
        {
            string substr;
            getline(word, substr, '#');
            v.push_back(substr);
        }
        vector<string> cols;
        for(int i = 1; i < v.size(); i+=2){
            cols.push_back(v[i]);
        }
        m[v[0]] = cols;
    }

    fin.close();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////  get schema details  //////////////////////////////////////////////////////
void get_schema(string t_Name, vector<string> &schema_p){
    fstream fin;
    fin.open("schema.txt", ios::in);

    string line;
    int flag = 0;

    while (getline(fin, line))
    {
        string l1 = line.substr(0, line.find('#'));
        if (t_Name == l1)
        {
            flag = 1;
            stringstream word(line);

            while (word.good())
            {
                string substr;
                getline(word, substr, '#');
                schema_p.push_back(substr);
            }
            break;
        }
    }

    if (flag == 0)
    {
        cout << "table not found" << endl;
    }

    fin.close();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////  create  //////////////////////////////////////////////////
void create(vector<string> query_create){
    ofstream fout, fout_table;

    string schema_path = "schema.txt";
    ifstream table(schema_path);
    string table_name1 = query_create[2];
    cout<<endl;

    bool exists = false;

    if(table.good()){
        while(table){
            string line;
            getline(table, line);
            cout.flush();

            if(line.substr(0, table_name1.length()) == table_name1){
                exists = true;
            }
        }
        table.close();
    }

    if(exists == true){
        cout<<"Table already exists!"<<endl;
    }
    else{
        vector<string> query;
        query = query_create;

        fout.open("schema.txt", ios_base::app);
        cout<<endl;

        string table_name = query_create[2]+".txt";
        fout_table.open(table_name, ios_base::app);

        fout<<query_create[2];
        for(int j = 3; j<query_create.size();j++){
            if(query_create[j] == "int"){
                fout<<"#"<<query_create[j-1]<<"#"<<query_create[j];
                //fout_table<<query_create[j-1]<<"#";
            }

            if(query_create[j] == "decimal"){
                fout<<"#"<<query_create[j-1]<<"#"<<query_create[j];
                //fout_table<<query_create[j-1]<<"#";
            }

            if(query_create[j].substr(0,4) == "char"){
                fout<<"#"<<query_create[j-1]<<"#"<<query_create[j];
                //fout_table<<query_create[j-1]<<"#";
            }
        }
        fout<<endl;
        //fout_table<<endl;
        fout.close();
        fout_table.close();
        cout<<"Table created successfully"<<endl;
        cout<<endl;
    }
}
////////////////////////////////////////////////  create done  /////////////////////////////////////////////////

//////////////////////////////////////////////////////////  describe  ////////////////////////////////////////////////
void describe(vector<string> query_describe){

    string schema_path = "schema.txt";
    ifstream table(schema_path);
    string table_name = query_describe[1];
    cout<<endl;
    int cn=0;

    if(table.good()){
        bool exists = false;

        while(table){
            string line;
            getline(table, line);
            cout.flush();
            //Checking for table
            if(line.substr(0, table_name.length()) == table_name){
                exists = true;
                for(int i = table_name.length()+1; i<line.length();i++){
                    if(line[i] == '#'){
                        cn++;
                        if(cn%2 == 0){
                            cout<<endl;
                        }
                        else{
                            cout<<" ---- ";
                        }
                        i++;
                    }
                    cout<<line[i];
                }
                cout<<endl;
            }
        }

        if(exists == false){
            cout<<"Table does not exists"<<endl;
        }
        table.close();
    }
    cout<<endl;
}
////////////////////////////////////////////////  describe done  ////////////////////////////////////////////////

///////////////////////////////////////////////////  insert  ///////////////////////////////////////////////
void insert(vector<string> query_insert){
    string schema_path = "schema.txt";
    ifstream table(schema_path);
    string table_name1 = query_insert[2];
    cout<<endl;
    int coun=0;

    bool exists = false;

    if(table.good()){
        while(table){
            string line;
            getline(table, line);
            cout.flush();
            coun++;
            //Checking for table
            if(line.substr(0, table_name1.length()) == table_name1){
                exists = true;
                break;
            }
        }
        table.close();
    }

    if(exists == false){
        cout<<"Table does not exists!"<<endl;
    }
    else{
        vector<string> v1;
        vector<string> v2;
        int coun=0;

        for(int i=5; i<query_insert.size()-1; i+=2){
            v1.push_back(query_insert[i]);
        }

        vector<string> schema_p;
        string it;
        int count_temp = 0;

        get_schema(query_insert[2],schema_p);

        for(int i=2; i<schema_p.size(); i+=2){
            coun++;
            v2.push_back(schema_p[i]);
        }

        int flag1 = 0;
        for(int i=0; i<v1.size(); i++){
            int flag = 0;
            string s = v1[i];
            for(int j=0; j<s.length(); j++){
                if(isalpha(s[j])){
                    flag = 1;
                    break;
                }
            }

            if(flag = 1){
                it = "char";
                if(it == v2[i]){
                    count_temp++;
                }
            }
            if(isdigit(s[0])){
                for(int j=0; j<s.length(); j++){
                    if(s[j] == '.'){
                        flag1 = 1;
                        break;
                    }
                }
                if(flag1 == 1){
                    it = "decimal";
                }
                else{
                    it = "int";
                }

                if(it == v2[i]){
                    count_temp++;
                }
            }
        }

        if(coun == count_temp){
            string table_name = query_insert[2];
            string tname=query_insert[2]+".txt";
            ofstream insert_table;
            insert_table.open(tname, ios_base::app);

            if(insert_table){
                for(int i=5;i<query_insert.size()-1;i++){
                    if(query_insert[i] != ",")
                    {
                        insert_table<<query_insert[i]<<"#";
                    }
                }
                insert_table<<endl;
                cout<<"Record Inserted Successfully!"<<endl;
            }
            else{
                cout<<"Table does not exists"<<endl;
            }
            insert_table.close();
        }
        else{
            cout<<"Error in query written!"<<endl;
        }
    }
}
////////////////////////////////////////////////  insert done  ////////////////////////////////////////////////

//////////////////////////////////////////////  drop table  //////////////////////////////////////////////////////
void drop(vector<string> query_drop){
    string schema_path = "schema.txt";
    ifstream table(schema_path);
    string table_name1 = query_drop[2];
    cout<<endl;
    int coun=0;

    bool exists = false;

    if(table.good()){
        while(table){
            string line;
            getline(table, line);
            cout.flush();
            coun++;
            //Checking for table
            if(line.substr(0, table_name1.length()) == table_name1){
                exists = true;
                break;
            }
        }
        table.close();
    }

    if(exists == false){
        cout<<"Table does not exists!"<<endl;
    }
    else{
        int status;
        //string table_name = query_drop[2];
        string tname=query_drop[2]+".txt";

        status = remove(tname.c_str());
        if(status==0){
            cout<<"\nTable drop Successfully!"<<endl;
        }
        else{
            cout<<"\nError Occurred!"<<endl;
        }
       //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        FILE *file, *temp;
        char buffer[MAX_LINE];

        file = fopen("schema.txt", "r");
        temp = fopen("temp.txt", "w");

        if(file==NULL || temp==NULL){
            cout<<"Error opening file(s)!"<<endl;
        }

        int keep_reading = true;
        int current_line = 1;

        do{
            fgets(buffer, MAX_LINE, file);
            if(feof(file)){
                keep_reading = false;
            }
            else if(current_line != coun){
                fputs(buffer,temp);
            }
            current_line++;
        }while(keep_reading);

        fclose(file);
        fclose(temp);

        remove("schema.txt");
        rename("temp.txt","schema.txt");
    }
}
////////////////////////////////////////////////  drop table done  ////////////////////////////////////////////////

//////////////////////////////////////////////  help tables  //////////////////////////////////////////////////////
void helptables(){
    ifstream inFile;
    inFile.open("schema.txt");

    string line, word;
    istringstream iss;

    while(!inFile.eof()){
        getline(inFile,line);

        if(inFile.good()){
            iss.clear();
            iss.str(line);
            while(iss.good()){
                iss>>word;
                for(char c : word){
                    if(c=='#'){
                        break;
                    }
                    cout<<c;
                }
                cout<<endl;
            }
        }
    }
    inFile.close();
}
////////////////////////////////////////////////  help tables done  ////////////////////////////////////////////////

////////////////////////////////////////////  select  ////////////////////////////////////////////////////
void select(vector<string> query_select){
    bool all = false;
    if(query_select[1] ==  "*"){
        all = true;
    }
    vector<string> cols_name;
    vector<string> table_names;

    int from_index = 0;
    int where_index = 0;
    int end_index = 0;

    for(int i =0;i < query_select.size();i++){
        if(query_select[i] == "from"){
            from_index =i;
        }
        if(query_select[i] == "where"){
            where_index = i;
        }
        if(query_select[i] == ";"){
            end_index = i;
        }
    }

    if(where_index==0){
        for(int i = from_index+1; i<query_select.size();i++){
            if(query_select[i] != ","){
                table_names.push_back(query_select[i]);
            }
        }
    }
    else{
        for(int i = from_index+1; i<where_index;i++){
            if(query_select[i] != ","){
                table_names.push_back(query_select[i]);
            }
        }
    }

    if(all == true){
        if(where_index == 0){
            string table_name = table_names[0];
            string table_path = table_name+".txt";
            ifstream table(table_path);
            cout<<endl;
            if(table.good()){

                while(table){
                    string line;
                    getline(table, line);
                    cout.flush();
                    for(int i=0; i<line.length(); i++){
                        if(line[i]=='#'){
                            cout<<" ";
                        }
                        else{
                            cout<<line[i];
                        }
                    }
                    cout<<endl;
                }
                table.close();
            }
            else{
                cout<<"Table does not exists"<<endl;
                return ;
            }
        }

        ///////////////////////////////////////////////  with where  /////////////////////////////////////////

        else{
            string table_name = table_names[0];
            string table_path = table_name+".txt";


            vector<string> schema_p;
            get_schema(table_name, schema_p);

            fstream tab1;
            string t_name = table_name+".txt";
            tab1.open(t_name, ios::in);
            int flag = 0, j=0;
            string line1;
            int coun=0;
            vector<string> wf_lines;

            for (int i = 1; i < schema_p.size(); i += 2){
                coun++;
                if (query_select[5] == schema_p[i]){
                    break;
                }
            }
            int res = coun;
            cout<<endl;

            while(getline(tab1, line1)){
                stringstream word1(line1);

                while(word1.good()){
                    string substr;
                    getline(word1, substr, '#');
                    wf_lines.push_back(substr);
                }

                if(query_select[6] == "="){
                    if(wf_lines[coun-1] == query_select[7]){
                        flag=1;
                    }
                }
                if(query_select[6] == ">"){
                    float num1 = stof(query_select[7]);
                    float num2 = stof(wf_lines[coun-1]);

                    if(num2 > num1){
                        flag=1;
                    }
                }
                if(query_select[6] == "<"){
                    float num1 = stof(query_select[7]);
                    float num2 = stof(wf_lines[coun-1]);

                    if(num2 < num1){
                        flag=1;
                    }
                }
                if(query_select[6] == ">="){
                    float num1 = stof(query_select[7]);
                    float num2 = stof(wf_lines[coun-1]);

                    if(num2 >= num1){
                        flag=1;
                    }
                }
                if(query_select[6] == "<="){
                    float num1 = stof(query_select[7]);
                    float num2 = stof(wf_lines[coun-1]);

                    if(num2 <= num1){
                        flag=1;
                    }
                }
                if(query_select[6] == "!="){
                    if(wf_lines[coun-1] != query_select[7]){
                        flag=1;
                    }
                }

                if (flag == 1)
                {
                    for(int i=0; i<line1.length(); i++){
                        if(line1[i]=='#'){
                            cout<<" ";
                        }
                        else{
                            cout<<line1[i];
                        }
                    }
                    cout<<endl;
                }

                flag = 0;
                coun = wf_lines.size()+res;
            }
            tab1.close();
        }
    }
    else{
        if(where_index==0){
            for(int i = 1; i<from_index;i++){
                if(query_select[i] != ","){
                    cols_name.push_back(query_select[i]);
                }
            }

            populate_map();

            string t_name1 = table_names[0];
            int line_count_1 = get_line_count(t_name1);
            vector<string> cols = m[t_name1];
            vector<int> col_index;

            for(int i = 0; i < cols.size(); i++){
                for(string s: cols_name){
                    if(s == cols[i])
                        col_index.push_back(i);
                }
            }

            string table_name1 = t_name1;
            string table_path1 = table_name1+".txt";
            ifstream table1(table_path1);
            int line_count_2=0;
            cout<<endl;
            if(table1.good()){

                while(!table1.eof()){
                    string line1;
                    getline(table1, line1);
                    line_count_2++;
                    cout.flush();
                    vector<string> row;
                    string s = "";
                    for(int i=0; i<line1.length(); i++){
                        if(line1[i]=='#'){
                            // cout<<" ";
                            row.push_back(s);
                            s = "";
                        }
                        else{
                            // cout<<line[i];
                            s+=line1[i];
                        }
                    }
                    for(int i = 0; i < col_index.size(); i++){
                        cout<<row[col_index[i]];
                        cout<<" ";
                    }
                    cout<<endl;
                    if(line_count_1 == line_count_2){
                        break;
                    }
                }
                table1.close();
            }
            else{
                cout<<"Table does not exists"<<endl;
                return ;
            }
        }

        ///////////////////////////////////////  with where  ////////////////////////////////////////////

        else{
            for(int i = 1; i<from_index; i+=2){
                cols_name.push_back(query_select[i]);
            }

            vector<string> schema_p;
            vector<int> count_col;
            vector<int> count_temp;
            get_schema(table_names[0], schema_p);

            int coun, coun_col_where=0;
            for (int j=0; j < cols_name.size(); j++){
                coun=0;
                for(int i=1; i<schema_p.size(); i+=2){
                    coun++;
                    if (cols_name[j] == schema_p[i]){
                        count_col.push_back(coun);
                        break;
                    }
                }
            }

            for(int i=0; i<count_col.size(); i++){
                count_temp.push_back(count_col[i]);
            }

            fstream tab1;
            string t_name = table_names[0]+".txt";
            tab1.open(t_name, ios::in);
            string line1;
            vector<string> wf_lines;

            for (int i = 1; i < schema_p.size(); i += 2){
                coun_col_where++;
                if (query_select[where_index+1] == schema_p[i]){
                    break;
                }
            }
            int res = coun_col_where;
            int flag = 0;
            cout<<endl;

            while(getline(tab1, line1)){
                stringstream word1(line1);
                while(word1.good()){
                    string substr;
                    getline(word1, substr, '#');
                    wf_lines.push_back(substr);
                }

                if(query_select[where_index+2] == "="){
                    if(wf_lines[coun_col_where-1] == query_select[where_index+3]){
                        flag=1;
                    }
                }

                if(query_select[where_index+2] == ">"){
                    float num1 = stof(wf_lines[coun_col_where-1]);
                    float num2 = stof(query_select[where_index+3]);
                    if(num1 > num2){
                        flag=1;
                    }
                }

                if(query_select[where_index+2] == "<"){
                    float num1 = stof(wf_lines[coun_col_where-1]);
                    float num2 = stof(query_select[where_index+3]);
                    if(num1 < num2){
                        flag=1;
                    }
                }

                if(query_select[where_index+2] == ">="){
                    float num1 = stof(wf_lines[coun_col_where-1]);
                    float num2 = stof(query_select[where_index+3]);
                    if(num1 >= num2){
                        flag=1;
                    }
                }

                if(query_select[where_index+2] == "<="){
                    float num1 = stof(wf_lines[coun_col_where-1]);
                    float num2 = stof(query_select[where_index+3]);
                    if(num1 <= num2){
                        flag=1;
                    }
                }

                if(query_select[where_index+2] == "!="){
                    if(wf_lines[coun_col_where-1] != query_select[where_index+3]){
                        flag=1;
                    }
                }

                if(flag == 1){
                    for(int i=0; i<count_col.size(); i++){
                        cout<<wf_lines[count_col[i]-1];
                        cout<<" ";
                    }
                    cout<<endl;
                }
                for(int i=0; i<count_col.size(); i++){
                    count_col[i] = count_temp[i]+wf_lines.size();
                }

                coun_col_where = wf_lines.size()+res;
                flag = 0;
            }
            tab1.close();
        }
    }
}
////////////////////////////////////////////////  select done  ////////////////////////////////////////////////

/////////////////////////////////////////  delete  ///////////////////////////////////////////////////////
void delete1(vector<string> query_delete){
    string schema_path = "schema.txt";
    ifstream table(schema_path);
    string table_name1 = query_delete[2];
    cout<<endl;

    bool exists = false;

    int and_index = 0;
    int or_index = 0;

    for(int i =0;i < query_delete.size();i++){
        if(query_delete[i] == "and"){
            and_index = i;
        }
        if(query_delete[i] == "or"){
            or_index = i;
        }
    }

    if(table.good()){
        while(table){
            string line;
            getline(table, line);
            cout.flush();
            //Checking for table
            if(line.substr(0, table_name1.length()) == table_name1){
                exists = true;
                break;
            }
        }
        table.close();
    }

    if(exists == false){
        cout<<"Table does not exists!"<<endl;
    }
    else{
        if(query_delete.size() == 3){
            string file_name = table_name1+".txt";
            ofstream ofs;
            ofs.open(file_name, ofstream::out | ofstream::trunc);
            ofs.close();
            cout<<"All records deleted successfully!"<<endl;
        }

        ///////////////////////////////////////////////  with where  //////////////////////////////////////////////////

        else if(query_delete[3]=="where"){
            if(and_index == 0 && or_index == 0){
                int row_count=0;
                vector<string> schema_p;
                int coun = 0;
                get_schema(table_name1, schema_p);

                fstream tab1, temp;
                string t_name = table_name1+".txt";
                tab1.open(t_name, ios::in);
                temp.open("temp.txt", ios::out);
                int flag = 0, j=0;
                string line1;
                vector<string> wf_lines;

                for (int i = 1; i < schema_p.size(); i += 2){
                    coun++;
                    if (query_delete[4] == schema_p[i]){
                        break;
                    }
                }
                int res = coun;

                while(getline(tab1, line1)){
                    stringstream word1(line1);

                    while(word1.good()){
                        string substr;
                        getline(word1, substr, '#');
                        wf_lines.push_back(substr);
                    }

                    if(query_delete[5] == "="){
                        if(wf_lines[coun-1] == query_delete[6]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 > num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 < num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 >= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 <= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!="){
                        if(wf_lines[coun-1] != query_delete[6]){
                            flag=1;
                            row_count++;
                        }
                    }

                    if (flag != 1)
                    {
                        temp<<line1<<endl;
                    }
                    flag = 0;
                    coun = wf_lines.size()+res;
                }

                tab1.close();
                temp.close();
                char c[t_name.size() + 1];
                strcpy(c, t_name.c_str());
                remove(c);
                rename("temp.txt", c);
                cout<<row_count<<" Rows affected!"<<endl;
            }
            else if(and_index != 0){
                int row_count=0;
                vector<string> schema_p;
                int coun = 0, coun1 = 0;
                get_schema(table_name1, schema_p);

                fstream tab1, temp;
                string t_name = table_name1+".txt";
                tab1.open(t_name, ios::in);
                temp.open("temp.txt", ios::out);
                int flag = 0, j=0;
                string line1;
                vector<string> wf_lines;

                for (int i = 1; i < schema_p.size(); i += 2){
                    coun++;
                    if (query_delete[4] == schema_p[i]){
                        break;
                    }
                }
                for (int i = 1; i < schema_p.size(); i += 2){
                    coun1++;
                    if (query_delete[and_index+1] == schema_p[i]){
                        break;
                    }
                }

                int res = coun;
                int res1 = coun1;

                while(getline(tab1, line1)){
                    stringstream word1(line1);

                    while(word1.good()){
                        string substr;
                        getline(word1, substr, '#');
                        wf_lines.push_back(substr);
                    }

                    ///////////////////////////////////////////////  ==  /////////////////////////////////////////////////////
                    if(query_delete[5] == "=" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] == query_delete[6] && num2 > num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] == query_delete[6] && num2 < num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] == query_delete[6] && num2 >= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] == query_delete[6] && num2 <= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == "!="){
                        if(wf_lines[coun-1] == query_delete[6] && wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == "="){
                        if(wf_lines[coun-1] == query_delete[6] && wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }

                    //////////////////////////////////////////////////  >  ////////////////////////////////////////////////////
                    if(query_delete[5] == ">" && query_delete[9] == "="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 > num1 && wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == "!="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 > num1 && wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 > num1) && (num4 > num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 > num1) && (num4 < num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 > num1) && (num4 >= num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 > num1) && (num4 <= num3)){
                            flag=1;
                            row_count++;
                        }
                    }

                    /////////////////////////////////////////////////  <  ///////////////////////////////////////////////////////
                    if(query_delete[5] == "<" && query_delete[9] == "="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 < num1 && wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == "!="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 < num1 && wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 < num1) && (num4 > num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 < num1) && (num4 < num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 < num1) && (num4 >= num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 < num1) && (num4 <= num3)){
                            flag=1;
                            row_count++;
                        }
                    }

                    /////////////////////////////////////////////////  >=  //////////////////////////////////////////////////////
                    if(query_delete[5] == ">=" && query_delete[9] == "="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 >= num1 && wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == "!="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 >= num1 && wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 >= num1) && (num4 > num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 >= num1) && (num4 < num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 >= num1) && (num4 >= num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 >= num1) && (num4 <= num3)){
                            flag=1;
                            row_count++;
                        }
                    }

                    ////////////////////////////////////////////////  <=  /////////////////////////////////////////////////////////
                    if(query_delete[5] == "<=" && query_delete[9] == "="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 <= num1 && wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == "!="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 <= num1 && wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 <= num1) && (num4 > num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 <= num1) && (num4 < num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 <= num1) && (num4 >= num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 <= num1) && (num4 <= num3)){
                            flag=1;
                            row_count++;
                        }
                    }

                    ///////////////////////////////////////////////  !=  ////////////////////////////////////////////////////////
                    if(query_delete[5] == "!=" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] != query_delete[6] && num2 > num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] != query_delete[6] && num2 < num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] != query_delete[6] && num2 >= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] != query_delete[6] && num2 <= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == "!="){
                        if(wf_lines[coun-1] != query_delete[6] && wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == "="){
                        if(wf_lines[coun-1] != query_delete[6] && wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

                    if (flag != 1)
                    {
                        temp<<line1<<endl;
                    }
                    flag = 0;
                    coun = wf_lines.size()+res;
                    coun1 = wf_lines.size()+res1;
                }

                tab1.close();
                temp.close();
                char c[t_name.size() + 1];
                strcpy(c, t_name.c_str());
                remove(c);
                rename("temp.txt", c);
                cout<<row_count<<" Rows affected!"<<endl;
            }
            else{
                int row_count=0;
                vector<string> schema_p;
                int coun = 0, coun1 = 0;
                get_schema(table_name1, schema_p);

                fstream tab1, temp;
                string t_name = table_name1+".txt";
                tab1.open(t_name, ios::in);
                temp.open("temp.txt", ios::out);
                int flag = 0, j=0;
                string line1;
                vector<string> wf_lines;

                for (int i = 1; i < schema_p.size(); i += 2){
                    coun++;
                    if (query_delete[4] == schema_p[i]){
                        break;
                    }
                }
                for (int i = 1; i < schema_p.size(); i += 2){
                    coun1++;
                    if (query_delete[and_index+1] == schema_p[i]){
                        break;
                    }
                }

                int res = coun;
                int res1 = coun1;

                while(getline(tab1, line1)){
                    stringstream word1(line1);

                    while(word1.good()){
                        string substr;
                        getline(word1, substr, '#');
                        wf_lines.push_back(substr);
                    }

                    ///////////////////////////////////////////////  =  /////////////////////////////////////////////////////
                    if(query_delete[5] == "=" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] == query_delete[6] || num2 > num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] == query_delete[6] || num2 < num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] == query_delete[6] || num2 >= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] == query_delete[6] || num2 <= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == "!="){
                        if(wf_lines[coun-1] == query_delete[6] || wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "=" && query_delete[9] == "="){
                        if(wf_lines[coun-1] == query_delete[6] || wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }

                    //////////////////////////////////////////////////  >  ////////////////////////////////////////////////////
                    if(query_delete[5] == ">" && query_delete[9] == "="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 > num1 || wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == "!="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 > num1 || wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 > num1) || (num4 > num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 > num1) || (num4 < num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 > num1) || (num4 >= num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 > num1) || (num4 <= num3)){
                            flag=1;
                            row_count++;
                        }
                    }

                    /////////////////////////////////////////////////  <  ///////////////////////////////////////////////////////
                    if(query_delete[5] == "<" && query_delete[9] == "="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 < num1 || wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == "!="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 < num1 || wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 < num1) || (num4 > num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 < num1) || (num4 < num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 < num1) || (num4 >= num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 < num1) || (num4 <= num3)){
                            flag=1;
                            row_count++;
                        }
                    }

                    /////////////////////////////////////////////////  >=  //////////////////////////////////////////////////////
                    if(query_delete[5] == ">=" && query_delete[9] == "="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 >= num1 || wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == "!="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 >= num1 || wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 >= num1) || (num4 > num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 >= num1) || (num4 < num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 >= num1) || (num4 >= num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == ">=" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 >= num1) || (num4 <= num3)){
                            flag=1;
                            row_count++;
                        }
                    }

                    ////////////////////////////////////////////////  <=  /////////////////////////////////////////////////////////
                    if(query_delete[5] == "<=" && query_delete[9] == "="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 <= num1 || wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == "!="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);

                        if(num2 <= num1 || wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 <= num1) || (num4 > num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 <= num1) || (num4 < num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 <= num1) || (num4 >= num3)){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "<=" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[6]);
                        float num2 = stof(wf_lines[coun-1]);
                        float num3 = stof(query_delete[10]);
                        float num4 = stof(wf_lines[coun1-1]);

                        if((num2 <= num1) || (num4 <= num3)){
                            flag=1;
                            row_count++;
                        }
                    }

                    ///////////////////////////////////////////////  !=  ////////////////////////////////////////////////////////
                    if(query_delete[5] == "!=" && query_delete[9] == ">"){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] != query_delete[6] || num2 > num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == "<"){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] != query_delete[6] || num2 < num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == ">="){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] != query_delete[6] || num2 >= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == "<="){
                        float num1 = stof(query_delete[10]);
                        float num2 = stof(wf_lines[coun1-1]);

                        if(wf_lines[coun-1] != query_delete[6] || num2 <= num1){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == "!="){
                        if(wf_lines[coun-1] != query_delete[6] || wf_lines[coun1-1] != query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    if(query_delete[5] == "!=" && query_delete[9] == "="){
                        if(wf_lines[coun-1] != query_delete[6] || wf_lines[coun1-1] == query_delete[10]){
                            flag=1;
                            row_count++;
                        }
                    }
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

                    if (flag != 1)
                    {
                        temp<<line1<<endl;
                    }
                    flag = 0;
                    coun = wf_lines.size()+res;
                    coun1 = wf_lines.size()+res1;
                }

                tab1.close();
                temp.close();
                char c[t_name.size() + 1];
                strcpy(c, t_name.c_str());
                remove(c);
                rename("temp.txt", c);
                cout<<row_count<<" Rows affected!"<<endl;
            }
        }
    }
}
////////////////////////////////////////////////  delete done  ////////////////////////////////////////////////

////////////////////////////////////////////////  update  ////////////////////////////////////////////////
void update(vector<string> query_update){
    string schema_path = "schema.txt";
    ifstream table(schema_path);
    string table_name1 = query_update[1];
    cout<<endl;
    vector<string> table_names;
    vector<string> values;
    vector<int> count_col;
    int row_count=0;

    bool exists = false;

    if(table.good()){
        while(table){
            string line;
            getline(table, line);
            cout.flush();
            //Checking for table
            if(line.substr(0, table_name1.length()) == table_name1){
                exists = true;
                break;
            }
        }
        table.close();
    }

    if(exists==false){
        cout<<"Table does not exists!"<<endl;
    }
    else{
        int where_index=0;
        int set_index=0;

        for(int i =0; i < query_update.size();i++){
            if(query_update[i] == "where"){
                where_index = i;
            }
            if(query_update[i] == "set"){
                set_index = i;
            }
        }

        fstream tab2;
        string line2;
        vector<string> wf_lines2;
        string t_name2 = table_name1+".txt";
        tab2.open(t_name2, ios::in);
        int wf_col=0;
        while(getline(tab2, line2)){
            stringstream word2(line2);

            while(word2.good()){
                string substr2;
                getline(word2, substr2, '#');
                wf_lines2.push_back(substr2);
            }

            for(int i=0; i<wf_lines2.size(); i++){
                wf_col++;
            }
            break;
        }
        tab2.close();

        if(where_index==0){
            for(int i = set_index+1; i<query_update.size();i+=4){
                table_names.push_back(query_update[i]);
            }
            for(int i = set_index+3; i<query_update.size();i+=4){
                values.push_back(query_update[i]);
            }

            vector<string> schema_p;
            vector<int> count_temp;
            get_schema(table_name1, schema_p);

            int coun;
            for (int j=0; j < table_names.size(); j++){
                coun=0;
                for(int i=1; i<schema_p.size(); i+=2){
                    coun++;
                    if (table_names[j] == schema_p[i]){
                        count_col.push_back(coun);
                        break;
                    }
                }
            }

            for(int i=0; i<count_col.size(); i++){
                count_temp.push_back(count_col[i]);
            }

            fstream tab1, temp;
            string t_name = table_name1+".txt";
            tab1.open(t_name, ios::in);
            temp.open("temp.txt", ios::out);
            string line1;
            vector<string> wf_lines;

            while(getline(tab1, line1)){
                stringstream word1(line1);
                row_count++;

                while(word1.good()){
                    string substr;
                    getline(word1, substr, '#');
                    wf_lines.push_back(substr);
                }

                for(int i=0; i<count_col.size(); i++){
                    wf_lines[count_col[i]-1] = values[i];
                }

                for(int i=0; i<count_col.size(); i++){
                    count_col[i] = count_temp[i]+wf_lines.size();
                }
            }

            for(int i=0; i<wf_lines.size(); i++){
                if(((i+1)%wf_col) == 0){
                    temp<<endl;
                }
                else{
                    temp<<wf_lines[i]<<"#";
                }
            }
            tab1.close();
            temp.close();
            char c[t_name.size() + 1];
            strcpy(c, t_name.c_str());
            remove(c);
            rename("temp.txt", c);
            cout<<row_count<<" Rows affected!"<<endl;
            //cout<<"Records Updated successfully!"<<endl;
        }

    ///////////////////////////////////////////////  with where  /////////////////////////////////////////

        else{
            for(int i = set_index+1; i<where_index; i+=4){
                table_names.push_back(query_update[i]);
            }
            for(int i = set_index+3; i<where_index; i+=4){
                values.push_back(query_update[i]);
            }

            vector<string> schema_p;
            vector<int> count_temp;
            get_schema(table_name1, schema_p);

            int coun, coun_col_where=0;
            for (int j=0; j < table_names.size(); j++){
                coun=0;
                for(int i=1; i<schema_p.size(); i+=2){
                    coun++;
                    if (table_names[j] == schema_p[i]){
                        count_col.push_back(coun);
                        break;
                    }
                }
            }

            for(int i=0; i<count_col.size(); i++){
                count_temp.push_back(count_col[i]);
            }

            fstream tab1, temp;
            string t_name = table_name1+".txt";
            tab1.open(t_name, ios::in);
            temp.open("temp.txt", ios::out);
            string line1;
            vector<string> wf_lines;

            for (int i = 1; i < schema_p.size(); i += 2){
                coun_col_where++;
                if (query_update[where_index+1] == schema_p[i]){
                    break;
                }
            }
            int res = coun_col_where;
            int flag = 0;

            while(getline(tab1, line1)){
                stringstream word1(line1);
                while(word1.good()){
                    string substr;
                    getline(word1, substr, '#');
                    wf_lines.push_back(substr);
                }

                if(query_update[where_index+2] == "="){
                    if(wf_lines[coun_col_where-1] == query_update[where_index+3]){
                        flag=1;
                        row_count++;
                    }
                }

                if(query_update[where_index+2] == ">"){
                    float num1 = stof(wf_lines[coun_col_where-1]);
                    float num2 = stof(query_update[where_index+3]);
                    if(num1 > num2){
                        flag=1;
                        row_count++;
                    }
                }

                if(query_update[where_index+2] == "<"){
                    float num1 = stof(wf_lines[coun_col_where-1]);
                    float num2 = stof(query_update[where_index+3]);
                    if(num1 < num2){
                        flag=1;
                        row_count++;
                    }
                }

                if(query_update[where_index+2] == ">="){
                    float num1 = stof(wf_lines[coun_col_where-1]);
                    float num2 = stof(query_update[where_index+3]);
                    if(num1 >= num2){
                        flag=1;
                        row_count++;
                    }
                }

                if(query_update[where_index+2] == "<="){
                    float num1 = stof(wf_lines[coun_col_where-1]);
                    float num2 = stof(query_update[where_index+3]);
                    if(num1 <= num2){
                        flag=1;
                        row_count++;
                    }
                }

                if(query_update[where_index+2] == "!="){
                    if(wf_lines[coun_col_where-1] != query_update[where_index+3]){
                        flag=1;
                        row_count++;
                    }
                }

                if(flag == 1){
                    for(int i=0; i<count_col.size(); i++){
                        wf_lines[count_col[i]-1] = values[i];
                    }
                }
                for(int i=0; i<count_col.size(); i++){
                    count_col[i] = count_temp[i]+wf_lines.size();
                }

                coun_col_where = wf_lines.size()+res;
                flag = 0;
            }

            for(int i=0; i<wf_lines.size(); i++){
                if(((i+1)%wf_col) == 0){
                    temp<<endl;
                }
                else{
                    temp<<wf_lines[i]<<"#";
                }
            }
            tab1.close();
            temp.close();
            char c[t_name.size() + 1];
            strcpy(c, t_name.c_str());
            remove(c);
            rename("temp.txt", c);
            cout<<row_count<<" Rows affected!"<<endl;
            //cout<<"Records Updated successfully!"<<endl;
        }
    }
}
////////////////////////////////////////////////  update done  ////////////////////////////////////////////////

////////////////////////////////////////////////  alter  //////////////////////////////////////////////////////
void alter(vector<string> query_alter){
    string schema_path = "schema.txt";
    ifstream table(schema_path);
    string table_name1 = query_alter[2];
    cout<<endl;

    bool exists = false;

    if(table.good()){
        while(table){
            string line;
            getline(table, line);
            cout.flush();

            if(line.substr(0, table_name1.length()) == table_name1){
                exists = true;
            }
        }
        table.close();
    }

    if(exists == false){
        cout<<"Table does not exists!"<<endl;
    }
    else{
        if(caseInSensStringCompare(query_alter[3], "add")){
            fstream fin,temp;
            fin.open("schema.txt", ios::in);
            temp.open("temp.txt", ios::out);
            string schema_name = "schema.txt";

            string line;
            int flag;

            while (getline(fin, line))
            {
                flag = 0;
                string l1 = line.substr(0, line.find('#'));
                if (table_name1 == l1)
                {
                    flag = 1;
                }

                if(flag==1){
                    temp<<line<<"#"<<query_alter[4]<<"#"<<query_alter[5]<<endl;
                }
                else{
                    temp<<line<<endl;
                }
            }
            fin.close();
            temp.close();
            char c[schema_name.size() + 1];
            strcpy(c, schema_name.c_str());
            remove(c);
            rename("temp.txt", c);

            ///////////////////////////////////////////////////////////////////////////////

            fstream t_name,temp1;
            string t_name1 = table_name1 + ".txt";
            t_name.open(t_name1, ios::in);
            temp1.open("temp1.txt", ios::out);

            string line1;

            while (getline(t_name, line1))
            {
                temp1<<line1<<"null"<<"#"<<endl;
            }
            t_name.close();
            temp1.close();
            char c1[t_name1.size() + 1];
            strcpy(c1, t_name1.c_str());
            remove(c1);
            rename("temp1.txt", c1);

            cout<<"Successfully executed alter command..."<<endl;
        }
        else if(caseInSensStringCompare(query_alter[3], "drop")){

        }
    }
}
////////////////////////////////////////////////  alter done  /////////////////////////////////////////////////

/////////////////////////////////////////  main  /////////////////////////////////////////////////////////
int main(){
    cout<<"Enter Query: ";
    string a;
    getline(cin,a);
    stringstream s1(a);
    string s2;
    vector<string> query;

    while(s1>>s2){
        query.push_back(s2);
    }

    while(query[0] != "quit"){
        int flag=0;

        if(caseInSensStringCompare(query[0],"create")){
            flag = 1;
            if(caseInSensStringCompare(query[1],"table")){
                if(query[3] == "(" && query[query.size()-1] == ")"){
                    int m=0;
                    for(int i=6; i<query.size()-2; i+=3){
                        if(query[i] != ","){
                            m = 1;
                        }
                    }
                    if(m == 0){
                        create(query);
                    }
                    else{
                        cout<<"Incorrect Query!"<<endl;
                    }
                }
                else{
                    cout<<"Incorrect Query!"<<endl;
                }
            }
            else{
                cout<<"Incorrect Query!"<<endl;
            }
        }
        else if(caseInSensStringCompare(query[0],"describe") || caseInSensStringCompare(query[0],"desc")){
            flag = 1;
            describe(query);
        }
        else if(caseInSensStringCompare(query[0],"insert")){
            flag = 1;
            if(caseInSensStringCompare(query[1],"into")){
                if(caseInSensStringCompare(query[3],"values")){
                    if(query[4] == "(" && query[query.size()-1] == ")"){
                        insert(query);
                    }
                    else{
                        cout<<"Incorrect Query!"<<endl;
                    }
                }
                else{
                    cout<<"Incorrect Query!"<<endl;
                }
            }
            else{
                cout<<"Incorrect Query!"<<endl;
            }
        }
        else if(caseInSensStringCompare(query[0],"drop")){
            flag=1;
            if(caseInSensStringCompare(query[1],"table")){
                drop(query);
            }
            else{
                cout<<"Incorrect Query!"<<endl;
            }
        }
        else if(caseInSensStringCompare(query[0],"help")){
            flag=1;
            if(caseInSensStringCompare(query[1],"tables")){
                int length;
                ifstream inFile;

                inFile.open("schema.txt");
                if(inFile){
                    inFile.seekg(0, ios::end);
                    length = inFile.tellg();

                    if ( length == 0 ){
                        cout<<"No tables found!"<<endl;
                    }
                    else{
                        inFile.close();
                        helptables();
                    }
                }
                else{
                    cout<<"No tables found!"<<endl;
                }
            }
            else if(caseInSensStringCompare(query[1],"create")){
                if(caseInSensStringCompare(query[2],"table")){
                    cout<<"Name: CREATE TABLE"<<endl<<endl;
                    cout<<"Description: The CREATE TABLE statement is used to create a new table in a database. "<<endl;
                    cout<<"             The column parameter specify the name of the columns of the table. "<<endl;
                    cout<<"             The datatype parameter specifies the type of data the column can hold (e.g. varchar, integer, date, etc.)."<<endl<<endl;
                    cout<<"Syntax: CREATE TABLE table_name (column1 datatype, column2 datatype, ........);"<<endl;
                }
                else{
                    cout<<"Incorrect Query!"<<endl;
                }
            }
            else if(caseInSensStringCompare(query[1],"drop")){
                if(caseInSensStringCompare(query[2],"table")){
                    cout<<"Name: DROP TABLE"<<endl<<endl;
                    cout<<"Description: The DROP TABLE statement is used to drop an existing table in a database."<<endl<<endl;
                    cout<<"Note: Be careful before dropping a table. Deleting a table will result in loss of complete information stored in the table!"<<endl<<endl;
                    cout<<"Synatx: DROP TABLE table_name;"<<endl;
                }
                else{
                    cout<<"Incorrect Query!"<<endl;
                }
            }
            else if(caseInSensStringCompare(query[1],"select")){
                cout<<"Name: SELECT"<<endl<<endl;
                cout<<"Description: The SELECT statement is used to select data from a database."<<endl;
                cout<<"             The data returned is stored in a result table, called the result-set."<<endl;
                cout<<"             Here, column1, column2, ... are the field names of the table you want to select data"<<endl<<endl;
                cout<<"1. If you want to select all the fields available in the table, use the following"<<endl;
                cout<<"Syntax: SELECT * FROM table_name;"<<endl<<endl;
                cout<<"2. If you want to select particular fields available in the table, use the following"<<endl;
                cout<<"Syntax: SELECT column1, column2,... FROM table_name;"<<endl<<endl;
            }
            else if(caseInSensStringCompare(query[1],"insert")){
                cout<<"Name: INSERT"<<endl<<endl;
                cout<<"Description: The INSERT INTO statement is used to insert new records in a table."<<endl<<endl;
                cout<<"It is possible to write the INSERT INTO statement in two ways:"<<endl<<endl;
                cout<<"1. Specify both the column names and the values to be inserted:"<<endl;
                cout<<"Syntax: INSERT INTO table_name (column1, column2, column3, ...) VALUES (value1, value2, value3, ...);"<<endl<<endl;
                cout<<"2. If you are adding values for all the columns of the table, you do not need to specify the column names in the SQL query."<<endl;
                cout<<"Syntax: INSERT INTO table_name VALUES (value1, value2, value3, ...);"<<endl<<endl;
            }
            else if(caseInSensStringCompare(query[1],"delete")){
                cout<<"Name: DELETE"<<endl<<endl;
                cout<<"Description: The DELETE statement is used to delete existing records in a table."<<endl<<endl;
                cout<<"Note: Be careful when deleting records in a table! Notice the WHERE clause in the DELETE statement. "
                "The WHERE clause specifies which record(s) should be deleted. If you omit the WHERE clause, all records in the table will be deleted!"<<endl<<endl;
                cout<<"Syntax: DELETE FROM table_name WHERE condition;"<<endl<<endl;
            }
            else if(caseInSensStringCompare(query[1],"update")){
                cout<<"Name: UPDATE"<<endl<<endl;
                cout<<"Description: The UPDATE statement is used to modify the existing records in a table."<<endl<<endl;
                cout<<"Note: Be careful when updating records in a table! Notice the WHERE clause in the UPDATE statement. The WHERE clause specifies which record(s) that should be updated. "
                "If you omit the WHERE clause, all records in the table will be updated!"<<endl<<endl;
                cout<<"Syntax: UPDATE table_name SET column1 = value1, column2 = value2, ... WHERE condition;"<<endl<<endl;
            }
            else{
                cout<<"Incorrect Query!"<<endl;
            }
        }
        else if(caseInSensStringCompare(query[0],"select")){
            flag=1;
            select(query);
        }
        else if(caseInSensStringCompare(query[0],"delete")){
            flag=1;
            if(caseInSensStringCompare(query[1],"from")){
                delete1(query);
            }
            else{
                cout<<"Incorrect Query!"<<endl;
            }
        }
        else if(caseInSensStringCompare(query[0],"update")){
            flag=1;
            if(caseInSensStringCompare(query[2],"set")){
                update(query);
            }
            else{
                cout<<"Incorrect Query!"<<endl;
            }
        }
        else if(caseInSensStringCompare(query[0],"alter")){
            flag=1;
            if(caseInSensStringCompare(query[1],"table")){
                alter(query);
            }
            else{
                cout<<"Incorrect Query!"<<endl;
            }
        }

        if(flag == 0){
            cout<<"Incorrect query!"<<endl;
        }

        query.clear();
        cout<<endl;
        cout<<"Enter query: ";
        string a;
        getline(cin,a);
        stringstream s1(a);
        string s2;
        while(s1>>s2){
            query.push_back(s2);
        }
    }
    return 0;
}
