# include<iostream>
# include<vector>
# include"Eigen/Eigen"
using namespace Eigen;
std::istream& operator >> (std::istream& in, Vector2d& p){
  in >> p.x() >> p.y();
  return in;
}
void input_vec(std::vector<Vector2d>& p_vec, int size){
  for(int i=0; i<size; i++){
    Vector2d p;
    std::cin >> p;
    p_vec.push_back(p);
  }
}
double calc_tangent(Vector2d e1, Vector2d e2){
  if(e1.norm() != 1 && e2.norm() !=1){
    e1.normalized();
    e2.normalized();
  }
  return (1-e1.dot(e2))/(e1.x()*e2.y() - e1.y()*e2.x());
}
double calc_weight(Vector2d const x, Vector2d const a, Vector2d const b, Vector2d const c){
  double w;
  double tan1, tan2;
  tan1 = calc_tangent(a-x,b-x);
  tan2 = calc_tangent(b-x,c-x);
  //std::cout << tan1 <<"  "<< tan2 <<"\n";
  w = (tan1 + tan2)/(b-x).norm();
  return w;
}
int main(){
  std::vector<Vector2d> porygon;
  int pory_size;
  std::cout << "porygon_size: "; std::cin >> pory_size;
  input_vec(porygon, pory_size);

  std::vector<Vector2d> point;
  int point_size;
  std::cout << "point_size: "; std::cin >> point_size;
  input_vec(point, point_size);

  std::cout << "input::ok\n";
  MatrixXd weight_mat(point_size, pory_size);
  for(int i=0; i<point_size; i++){
    double weight_sum = 0;
    for(int j=0; j<pory_size; j++){
      double weight = 0;
      if(j == 0){
        weight = calc_weight(point[i], porygon[pory_size-1], porygon[j], porygon[j+1]);
      }else{
        weight = calc_weight(point[i], porygon[j-1], porygon[j], porygon[(j+1)%pory_size]);
      }
      std::cout << weight <<"\n";
      weight_sum += weight;
      weight_mat(i,j) = weight;
    }
    for(int j=0; j<pory_size; j++){
      weight_mat(i,j) /= weight_sum;
    }
  }

  std::cout << "weight::ok\n";

  std::cout << "input new porygon\n";
  std::vector<Vector2d> move_pory;
  input_vec(move_pory, pory_size);

  MatrixXd move_pory_x(pory_size, 1);
  MatrixXd move_pory_y(pory_size, 1);
  for(int i=0; i<pory_size; i++){
    move_pory_x(i,0) = move_pory[i].x();
    move_pory_y(i,0) = move_pory[i].y();
  };

  MatrixXd result_x = weight_mat * move_pory_x;
  MatrixXd result_y = weight_mat * move_pory_y;
  MatrixXd result(point_size, 2); result << result_x, result_y;

  std::cout << "result\n";
  std::cout << result <<"\n";

  return 0;
}
