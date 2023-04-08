#include <stdio.h>

#define ROWSIZE 64

class Row {
  public:
    Row(unsigned a, Row *n): address(a), next(n)  {
      for (int i = 0; i < ROWSIZE; i++) data[i] = 0xff;
    }
    Row(unsigned a): Row(a, this) {}
    Row *check_address(unsigned a, Row *last) {
      if (a == address) return this;
      if (next != last) return next->check_address(a, last);
      return next = new Row(a, last);
    }
    Row *find_address(unsigned a, Row *last) {
      if (next->address == a) return this;
      return next == last ? 0 : next->find_address(a, last);
    }
    Row *dump(Row *last) {
      printf("address= %08x:\n", address);
      for (int i = 0; i < ROWSIZE; i+=16) {
        for (int j = 0; j < 16; j++) printf("%02x", data[i + j]);
        printf("\n");
      }
      if (!last || (last == next)) return this;
      return next->dump(last);
    }
    Row *get_next(void) { return next; }
    Row *remove_next(void) {
      Row *n = next;
      next = next->next;
      return n;
    }
    unsigned save_data(unsigned char c, unsigned i) {
      data[i] = c;
      return address;
    }
    unsigned get_address(void) { return address; }
    void get_data(unsigned char *buf) {
      for (int i = 0; i < ROWSIZE; i++) buf[i] = data[i];
    }
  private:
    unsigned address;
    Row *next;
    unsigned char data[ROWSIZE];
};

namespace
{

Row *row;
unsigned row_index;

} //anonymous

unsigned get_start_address(void) {
  return row->get_next()->get_address();
}

void set_row_address(unsigned a) {
  unsigned addr = a & ~(ROWSIZE - 1);
  if (!row) row = new Row(addr);
  row = row->get_next();
  row = row->check_address(addr, row);
  row_index = a & (ROWSIZE - 1);
}

void save_row_data(unsigned char c) {
  unsigned a = row->save_data(c, row_index++);
  if (row_index == ROWSIZE) set_row_address(a + ROWSIZE);
}

void dump_rows(void) {
  if (row) {
    row = row->get_next();
    row = row->dump(row);
  }
}

unsigned pop_row(unsigned addr, unsigned char *buf) {
  for (int i = 0; i < ROWSIZE; i++) buf[i] = 0xff;
  if (!row) return 0;
  Row *r = row->find_address(addr, row);
  if (r) {
    r = r->remove_next();
    if (row == r) row = 0;
    r->get_data(buf);
    delete r;
    return addr;
  } else return 0;
}

unsigned pop_row(void) {
  if (!row) return 0;
  Row *r = row->remove_next();
  unsigned address = r->get_address();
  if (r == row) row = 0;
  delete r;
  return address;
}

/*** MAIN ******************************************************
int main(void) {
  set_row_address(0x1d000000);
  set_row_address(0x1d000080);
  set_row_address(0x1d000170);
  for (int i = 0; i < 256; i++) save_row_data(i & 255);
  row = row->get_next();
  row->dump(row);
  return 0;
}
// ** MAIN *****************************************************/
