/*
  実行までの例（Eigenのパスは自身のパスを指定してください）
  g++ wp_coord.cpp -o mv_coord -std=c++17 -I /usr/local/.../include/eigen3
  ./wp_coord
  
  注意：gnuplot.hppは改良中なのでまだあげてません．
*/
# include<iostream>
# include<vector>
# include<Eigen/Core>
# include<Eigen/LU>
# include"gnuplot.hpp"
using namespace Eigen;
std::istream& operator >> (std::istream& in, Vector2d& p){
  in >> p.x() >> p.y();
  return in;
}
double calc_tri_area(Vector2d a, Vector2d b, Vector2d c){
  Matrix3d A;
  A << 1,1,1, a.x(),b.x(),c.x(), a.y(),b.y(),c.y();
  double area; 
  area = A.determinant()/2.0;
  //std::cout <<"area"<< A <<"\n";
  return area;
}
void input_vec(std::vector<Vector2d>& p_vec, int size){
  for(int i=0; i<size; i++){
    Vector2d p;
    std::cin >> p;
    p_vec.push_back(p);
  }
}
struct Point{
  double x;
  double y;
};
int main(){
  std::vector<Vector2d> porygon;
  std::vector<Point> pory;
  int pory_size;
  std::cout << "porygon_size: "; std::cin >> pory_size;
  input_vec(porygon, pory_size);

  std::vector<Vector2d> point;
  std::vector<Point> poi;
  int point_size;
  std::cout << "point_size: "; std::cin >> point_size;
  input_vec(point, point_size);

  for(auto p: porygon){
    pory.push_back({p.x(), p.y()});
  };
  for(auto p: point){
    poi.push_back({p.x(), p.y()});
  };

  Gnuplot plotter_;
  plotter_.add_polygon(pory);
  plotter_.add_points(poi);
  plotter_.draw();

  std::cout << "input::ok\n";
  MatrixXd weight_mat(point_size, pory_size);
  for(int i=0; i<point_size; i++){
    double weight_sum = 0;
    for(int j=0; j<pory_size; j++){
      double area_B, area_A1, area_A2;
      if(j == 0){
        area_B = calc_tri_area(porygon[pory_size-1], porygon[j], porygon[j+1]);
        area_A1 = calc_tri_area(point[i], porygon[pory_size-1], porygon[j]);
        area_A2 = calc_tri_area(point[i], porygon[j], porygon[j+1]);
      }else{
        area_B = calc_tri_area(porygon[j-1], porygon[j], porygon[(j+1)%pory_size]);
        area_A1 = calc_tri_area(point[i], porygon[j-1], porygon[j]);
        area_A2 = calc_tri_area(point[i], porygon[j], porygon[(j+1)%pory_size]);
      }
      double weight = area_B / (area_A1 * area_A2);
      //std::cout << weight <<"\n";
      weight_sum += weight;
      weight_mat(i,j) = weight;
    }
    for(int j=0; j<pory_size; j++){
      weight_mat(i,j) /= weight_sum;
    }
  }
  std::cout << "weight::ok\n";
  std::cout << weight_mat <<"\n";

  std::cout << "input new porygon\n";
  std::vector<Vector2d> move_pory;
  input_vec(move_pory, pory_size);

  MatrixXd move_pory_x(pory_size, 1);
  MatrixXd move_pory_y(pory_size, 1);
  std::vector<Point> porygon_;
  for(int i=0; i<pory_size; i++){
    double x,y;
    x = move_pory[i].x();
    y = move_pory[i].y();
    move_pory_x(i,0) = x;
    move_pory_y(i,0) = y;
    porygon_.push_back({x,y});
  };

  std::vector<Point> point_;
  MatrixXd result_x = weight_mat * move_pory_x;
  MatrixXd result_y = weight_mat * move_pory_y;
  MatrixXd result(point_size, 2); result << result_x, result_y;
  std::cout <<"check\n";
  for(int i=0; i<point_size; i++){
    point_.push_back({result(i,0), result(i,1)});
    std::cout << point_[i].x <<" "<<point_[i].y <<"\n";
  }

  std::cout << "result\n";
  std::cout << result <<"\n";


//gnuplotで描画
  
  Gnuplot plotter;
  plotter.add_polygon(porygon_);
  plotter.add_points(point_);
  plotter.draw();

  return 0;
}
