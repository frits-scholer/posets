/*
Posets
*/
#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <set>

using namespace std;

#define all(c) begin(c), end(c)

typedef long Uint;
typedef pair<Uint, double> crabc;
typedef pair<Uint, Uint> radic;
typedef vector<vector<Uint>>::const_iterator chmrg_it;
inline bool even(Uint n) {return !(n&1);}
inline bool odd(Uint n) {return n&1;}

bool rel_prime(Uint m, Uint n) {
  while (even(m)) m >>= 1;
  while (even(n)) n >>= 1;
  while (m != n) {
    if (n > m) swap(n, m);
    m -= n;
    do m >>= 1; while (even(m));
  }
  return m==1;
}

void fill_radical(vector<Uint>& rad) {
  for (size_t i=2;i<rad.size();i++) {
    if (rad[i] == 1) {
      rad[i] = i;
      for (size_t j = i+i; j < rad.size(); j += i) rad[j] *= i;
    }
  }
}

bool comp(const crabc& a, const crabc& b) {
  return a.first <= b.first && a.second <= b.second;
}

ostream& operator << (ostream& out, const crabc& c) {
  return out << setprecision(10) << setw(2) << c.first << ", " <<  setw(12) << c.second;
}
void print_chainmerge(chmrg_it iv) {
  for (auto n : *iv) cout << n << " ";
  cout << endl;
}

char partial_relation(chmrg_it iv1, chmrg_it iv2) {
  char cur_rel = '=';
  for (size_t i=0;i<(*iv1).size();i++) {
    if ((*iv1)[i] < (*iv2)[i]) {
      if (cur_rel == '=') cur_rel = '<';
      if (cur_rel == '>') return '~';
    }
    else {
      if ((*iv1)[i] > (*iv2)[i]) {
	if (cur_rel == '=') cur_rel = '>';
	if (cur_rel == '<') return '~';
      }
    }
  }
  return cur_rel;
}

int main() {
  Uint Lmax = 100;
  vector<Uint> rads(Lmax,1);
  fill_radical(rads);
  vector<radic> srads(Lmax-1);
  int p{0};
  generate(all(srads),[&]{++p;return radic(rads[p],p);});
  sort(all(srads));
  set<crabc> P;
  for (Uint c=4;c<Lmax;++c) {
    double cl = log(c);
    double crl = c*sqrt(c)/rads[c];
    Uint sl = distance(begin(srads), upper_bound(all(srads),
						 radic(crl/2.0+1, 0)));
    if (odd(c)) {
      for (auto i=0;i<sl;i++) 
	if (2*srads[i].second < c) {
	  Uint rabc = srads[i].first*rads[c-srads[i].second];
	  if (rabc < crl && rel_prime(srads[i].first, rads[c]))
	    P.insert(crabc(c,log(rabc*rads[c])/cl));
	}
    }
    else {
      for (auto i=0;i<sl;i++) 
	if (odd(srads[i].first) && 2*srads[i].second < c) {
	  Uint rabc = srads[i].first*rads[c-srads[i].second];
	  if (rabc < crl && rel_prime(srads[i].first, rads[c]))
	    P.insert(crabc(c,log(rabc*rads[c])/cl));
	}
    }
  }
  //for_each(all(P),[](const crabc& c){cout << c << endl;});
  //cout << endl;
  vector<vector<crabc> > C;
  for (auto it=begin(P);it != end(P);++it) {
    bool failure=true;
    for (size_t i=0;i<C.size();++i) {
      if (comp(C[i].back(), *it)) {
	failure=false;
	C[i].push_back(*it);
	break;
      }
    }
      if (failure) C.push_back(vector<crabc>(1,*it));//increase width
  }
  vector<vector<Uint>> ChainMerge(P.size(), vector<Uint>(C.size()));
  size_t j=0;
  for_each(all(P), [&](const crabc& e){
      for (size_t i=0;i<C.size();i++) {
	ChainMerge[j][i]= distance(begin(C[i]), upper_bound(all(C[i]), e, comp))+1;
      }
      j++;
    });
 size_t i;
 while (true) {
   cout << "Enter 2 numbers between 1 and " << P.size() << ": ";
   cin >> i >> j;
   if (i<1 || i>P.size()) break;
   auto it1=begin(P);
   auto it2=begin(P);
   advance(it1,i-1);
   advance(it2,j-1);
   cout << *it1 << "\t";
   //print_chainmerge(begin(ChainMerge)+i-1) ;
   cout << partial_relation(begin(ChainMerge)+i-1, begin(ChainMerge)+j-1) << "\t";
   cout << *it2 << endl;
   //print_chainmerge(begin(ChainMerge)+j-1) ;

 }
}
