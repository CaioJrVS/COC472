/* A pure C/C++ version of a Gauss-Siedel Laplacian solver to test the
speed of a C program versus that of doing it with
Python/Numeric/Weave. */
#include<cstring>
#include <iostream>
#include <cmath>
#include <time.h>
typedef double Real;
inline double seconds(void)
{
    static const double secs_per_tick = 1.0 / CLOCKS_PER_SEC;
    return ((double)clock()) * secs_per_tick;
}
inline Real SQR(const Real &x)
{
    return (x * x);
}
inline Real BC(Real x, Real y)
{
    return (x * x - y * y);
}

struct Grid
{
    Real dx, dy;
    int nx, ny;
    Real *u;
    Grid(const int n_x = 10, const int n_y = 10);
    ~Grid();
    void setBCFunc(Real (*f)(const Real, const Real));
    /*Real computeError();*/
};
Grid ::Grid(const int n_x, const int n_y) : nx(n_x), ny(n_y)
{
        dx = 1.0/Real(nx - 1);
    dy = 1.0/Real(ny - 1);

    u = new Real[nx*ny];
    // for (int i=0; i<nx; ++i) {
    //     u[i] = new double [ny];
    // }
    //alterado aqui
    memset (u, 0, sizeof(u) );
    // int k;
    // for (int i=0; i<nx; ++i) {
    //     k = i*ny;
    //     for (int j=0; j<ny; ++j) {
    //         u[k+j] = 0.0;
    //     }
    // }
}

Grid ::~Grid()
{
    delete[] u;
}
void Grid ::setBCFunc(Real (*f)(const Real, const Real))
{
    Real xmin, ymin, xmax, ymax, x, y;
    xmin = 0.0;
    ymin = 0.0;
    xmax = 1.0;
    ymax = 1.0;
    int n_minus_1 = nx-1;
    /* Left and right sides. */
    for (int j=0; j<ny; ++j) {
        y = j*dy;
        u[j] = f(xmin, y);
        u[n_minus_1 + j] = f(xmax, y);
    }
    /* Top and bottom sides. */
    n_minus_1 = ny-1;
    int mul = 0;
    for (int i=0; i<nx; ++i) {
        x = i*dx;
        mul = i*ny;
        u[mul] = f(x, ymin);
        u[mul + n_minus_1] = f(x, ymax);
    }
}

struct LaplaceSolver
{
    Grid *g;
    LaplaceSolver(Grid *g);
    ~LaplaceSolver();
    void initialize();
    Real timeStep(const Real dt = 0.0);
    Real solve(const int n_iter = 0, const Real eps = 1e-16);
};
LaplaceSolver ::LaplaceSolver(Grid *grid)
{
    g = grid;
    initialize();
}
LaplaceSolver::~LaplaceSolver()
{
}

void LaplaceSolver ::initialize()
{
}
Real LaplaceSolver :: timeStep(const Real dt)
{
    Real dx2 = g->dx*g->dx;
    Real dy2 = g->dy*g->dy;
    Real tmp;
    Real err = 0.0;
    int nx = g->nx;
    int ny = g->ny;
    int k = 0;
    int l = 0;
    // x[i,j] = x[k] 
    // k = j_max*i + j 
    //alterado aqui
    Real *u = g->u;
    //alterado aqui
    Real sum = dx2 + dy2;
    //alterado aqui
    Real division = 1/(2*sum);
    for (int i=1; i<nx-1; ++i) {
        k = ny*i;
        for (int j=1; j<ny-1; ++j) {
            tmp = u[k];
            l = k + j;
            // u[k] = ((u[k-j_max + j] + u[k + j_max + j])*dy2 +
            //            (u[k + j - 1] + u[k + j + 1])*dx2)*0.5/sum;
            u[k] = ((u[l-ny] + u[l + ny])*dy2 +
                       (u[l - 1] + u[l + 1])*dx2)*division;
            err += (u[l] - tmp)*(u[l] - tmp);            
        }
    }
    return sqrt(err);
}
Real LaplaceSolver ::solve(const int n_iter, const Real eps)
{
    Real err = timeStep();
    int count = 1;
    while (err > eps)
    {
        if (n_iter && (count >= n_iter))
        {
            return err;
        }
        err = timeStep();
        ++count;
    }
    return Real(count);
}

int main(int argc, char *argv[])
{
    int nx, n_iter;
    Real eps;
    Real t_start, t_end;
    std::cout << "Enter nx n_iter eps --> ";
    std::cin >> nx >> n_iter >> eps;
    Grid *g = new Grid(nx, nx);
    g->setBCFunc(BC);
    LaplaceSolver s = LaplaceSolver(g);
    std::cout << "nx = " << g->nx << ", ny = " << g->ny << ", n_iter = " << n_iter << ", eps = " << eps << std::endl;
    t_start = seconds();
    std::cout << s.solve(n_iter, eps) << std::endl;
    t_end = seconds();
    std::cout << "Iterations took " << t_end - t_start << " seconds.\n ";
    return 0;
}