#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef std::vector<int> int_vec;

class string_search
{
  int_vec shifts;
  void compute_shifts(const std::string &pattern);
public:
  int find_first(const std::string &text, const std::string &pattern);
  int_vec find_all(const std::string &text, const std::string &pattern);

};
void string_search::compute_shifts(const std::string &pattern)
{
  int next_shift = 0;
  shifts.clear();
  shifts.push_back(0); // shift to the first character

  for(int i = 1; i < pattern.length(); i++)
  {
    while(next_shift > 0 && pattern[next_shift] != pattern[i])
      next_shift = shifts[next_shift];

    if(pattern[next_shift] == pattern[i])
      next_shift++;

    shifts.push_back(next_shift);
  }
}

int string_search::find_first(const std::string &text, const std::string &pattern)
{
  int next_shift = 0;
  compute_shifts(pattern);
  for(int i = 0; i < text.length(); i++)
  {
    while(next_shift > 0 && pattern[next_shift] != text[i])
      next_shift = shifts[next_shift - 1];

    if(pattern[next_shift] == text[i])
      next_shift++;

    if(next_shift == pattern.length())
      return i - (pattern.length() - 1); // found the first so return
  }
  return -1;
}

int_vec string_search::find_all(const std::string &text, const std::string &pattern)
{
  int next_shift = 0;
  int_vec positions;
  compute_shifts(pattern);
  for(int i = 0; i < text.length(); i++)
  {
    while(next_shift > 0 && pattern[next_shift] != text[i])
      next_shift = shifts[next_shift - 1];

    if(pattern[next_shift] == text[i])
      next_shift++;

    if(next_shift == pattern.length())
    {
      positions.push_back(i - (pattern.length() - 1));
      next_shift = shifts[next_shift - 1]; 
    }
  }
  return positions;
}

int main(int argc, char **argv)
{
  if(argc <= 2){
    cout << "Usage: " << argv[0] << " filename searchpattern" << endl;
    return 0;
  }
  std::string pattern = argv[2];

  fstream fs;
  std::string text, temp;
  fs.open(argv[1], ios::in);
  while(!fs.eof()){
    fs >> temp;
    text += temp;
  }
  fs.close();

  string_search search;
  int_vec pos_list = search.find_all(text, pattern);

  std::vector<int>::iterator it;
  cout << "Found " << pos_list.size() << " occurrences" << endl;
  for(it = pos_list.begin(); it != pos_list.end(); it++){
    temp = text.substr(*it, pattern.length());
    cout << "Pos=" << *it << " == " << temp << endl;
  }
}
