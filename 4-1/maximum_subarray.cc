#include <limits>
#include <chrono>
#include <random>
#include <iostream>



int maximum_subarray_recursive(const int* a, int l, int r) {
  //std::cout << l << " " << r << std::endl;
  int s = r - l;
  if (s == 0) {
    return a[l];
  }

  
  int best_left = maximum_subarray_recursive(a, l, l + s/2);
  int best_right = maximum_subarray_recursive(a, l + s/2 + 1, r);

  int greatest_left_sum = a[l + s/2 - 1];
  int current_left_sum = 0;
  for(int i = l + s/2 - 1; i >= l; --i) {
    current_left_sum += a[i];
    if (current_left_sum > greatest_left_sum){
      greatest_left_sum = current_left_sum;
    }
  }

  int greatest_right_sum = a[l + s/2];
  int current_right_sum = 0;
  for(int i = l + s/2; i <= r; ++i){
    current_right_sum += a[i];
    if (current_right_sum > greatest_right_sum) {
      greatest_right_sum = current_right_sum;
    }
  }

  int best_crossing = greatest_left_sum + greatest_right_sum;

  if (best_left >= best_right && best_left >= best_crossing)
    return best_left;

  if (best_right >= best_left && best_right >= best_crossing)
    return best_right;

  return best_crossing;
    
}

int maximum_subarray_recursive(const int* a, int n){
  return maximum_subarray_recursive(a, 0, n-1);  
}

int maximum_subarray_brute(const int* a, int l, int r) {
  int best = std::numeric_limits<int>::min();

  for(int i = l; i <= r; ++i){
    int current = 0;
    for(int j = 0; i + j <= r; ++j){
      current += a[i+j];
      if (current > best) {
	best = current;
      }
    }
  }

  return best;  
}

int maximum_subarray_brute(const int* a, int n) {
  return maximum_subarray_brute(a, 0, n - 1);
}



int maximum_subarray_mixed(const int* a, int l, int r) {
    //std::cout << l << " " << r << std::endl;
  int s = r - l;
  if (s < 32) {
    return maximum_subarray_brute(a, l, r);
  }

  
  int best_left = maximum_subarray_recursive(a, l, l + s/2);
  int best_right = maximum_subarray_recursive(a, l + s/2 + 1, r);

  int greatest_left_sum = a[l + s/2 - 1];
  int current_left_sum = 0;
  for(int i = l + s/2 - 1; i >= l; --i) {
    current_left_sum += a[i];
    if (current_left_sum > greatest_left_sum){
      greatest_left_sum = current_left_sum;
    }
  }

  int greatest_right_sum = a[l + s/2];
  int current_right_sum = 0;
  for(int i = l + s/2; i <= r; ++i){
    current_right_sum += a[i];
    if (current_right_sum > greatest_right_sum) {
      greatest_right_sum = current_right_sum;
    }
  }

  int best_crossing = greatest_left_sum + greatest_right_sum;

  if (best_left >= best_right && best_left >= best_crossing)
    return best_left;

  if (best_right >= best_left && best_right >= best_crossing)
    return best_right;

  return best_crossing;
}

int maximum_subarray_mixed(const int* a, int size) {
  return maximum_subarray_mixed(a, 0, size - 1);
}

int maximum_subarray_linear(const int* a, int size) {

  int current_best = a[0];
  int current_sum = a[0];
  int best_l = 0;
  int best_r = 0;

  int best_finishing_in_i = a[0];
  int finishing_in_i_l = 0;

  for (int i = 1; i < size; ++i) {
    if (best_finishing_in_i < 0) {
      best_finishing_in_i = a[i];
      finishing_in_i_l = i;
    } else {
      best_finishing_in_i += a[i];
    }

    current_sum += a[i];
    if (current_sum > best_finishing_in_i && current_sum > current_best) {
      current_best = current_sum;	
      best_r = i;
    }

    if (best_finishing_in_i > current_best){
      current_best = best_finishing_in_i;
      current_sum = best_finishing_in_i;
      best_l = finishing_in_i_l;
      best_r = i;      
    }    
    
  }

  //std::cout << "l: " << l << "  r: " << r << std::endl;

  return current_best;

}

int* random_array(int size){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(-100, 100);

  int* data = new int[size];
  for(int i = 0; i < size; ++i) {
    data[i] = dis(gen);
  }

  return data;
}

struct Timer {

  double& seconds;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> end;
  
  Timer(double& seconds) : seconds(seconds) {
    start = std::chrono::high_resolution_clock::now();      
  }

  virtual ~Timer() {
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    seconds = elapsed_seconds.count();    
  }

};

int main() {
  double recursive_time, brute_time, mixed_time, linear_time;

  int s = 256;
  int i = 0;
  while (s < (1 << 15)) {
    int* d = random_array(s);
    // for(int i = 0; i < s; ++i) {
    //   std::cout << d[i] << " ";      
    // }
    // std::cout << std::endl;

    int r1;
    {
      Timer bt(brute_time);
      r1 = maximum_subarray_brute(d, s);
    }

    int r2;
    {
      Timer br(recursive_time);
      r2 = maximum_subarray_recursive(d, s);
    }

    int r3;
    {
      Timer mr(mixed_time);
      r3 = maximum_subarray_mixed(d, s);

    }

    int r4;
    {
      Timer lt(linear_time);
      r4 = maximum_subarray_linear(d, s);

    }
      
    std::cout << std::endl;
    
    std::cout << "size: " << s << std::endl;
    std::cout << "brute: " << r1 << ". Time: " << brute_time << "s\n";
    std::cout << "recursive:" << r2 << ". Time: " << recursive_time << "s\n";
    std::cout << "mixed:" << r3 << ". Time: " << mixed_time << "s\n";
    std::cout << "linear:" << r4 << ". Time: " << linear_time << "s\n";
    std::cout << std::endl;
  

    s = s << 1;    
    delete d;

    std::cout << ".";
  }
  

}
