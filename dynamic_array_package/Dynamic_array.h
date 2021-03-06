#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

using namespace std;
class Dynamic_array
{
  public:
    Dynamic_array() {
      array_size = 256;
      array = new char[array_size];
      for(int i = 0; i < 256; i++) {
        array[i] = '\0';
      }
    }

    int size() {
      return last;
    }

    void insert(char c) {
      array[last++] = c;
      if(last == array_size)
        duplicate_size();
    }

    void insert(string token){
      for (int i = 0; i < token.size(); i++)
        insert(token[i]);
    }

    char get_at(int i) {
      return array[i];
    }

    char *get_array() {
      return array;
    }

    ~Dynamic_array() {
      delete array;
    }
  private:
    char *array;
    int last = 0;
    int array_size = 0;

    void copy(char* old_array, char* new_array) {
        for(int i = 0; i < last; i++)
            new_array[i] = old_array[i];
    }

    void duplicate_size() {
      array_size *= 2;
      char* new_array = new char[array_size];
      copy(array, new_array);
      delete array;
      array = new_array;
    }
};

#endif // DYNAMICARRAY_H
