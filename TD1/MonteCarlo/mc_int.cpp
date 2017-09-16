#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

#include "gnuplot_i.hpp"

using namespace std;

double int_mc(double(*)(double), double&, double, double, int);
double var_anal(double, double, double);
double f(double);
void wait_for_key ();

int main()
{
    Gnuplot g("lines");
    double a = 0.0;              // left endpoint
    double b = 1.0;               // right endpoint
    int N = 500;

    double varest, mc;
    std::vector<double> x, err, var_e, var_t, ecart_type;

    for (int n=5; n <= N; n++)
    {
        mc = int_mc(f, varest, a, b, n);

        x.push_back(n);
        err.push_back(fabs(mc-1.f));
        ecart_type.push_back(sqrt(var_anal(n, a, b)));
        var_t.push_back(sqrt(varest));
    }

    g.reset_plot();
    g.set_grid();
    g.set_style("lines").plot_xy(x,err,"erreur");
    g.set_style("lines").plot_xy(x,ecart_type,"ecart-type analytique");
    g.set_style("lines").plot_xy(x,var_t,"Ecar-type estim");

    wait_for_key();

    return 0;
}

/*
    Fonction à intégrer
*/
    double f(double x)
{
    double y = (5*(x*x*x*x));
    return y;
}

double var_anal(double n, double a, double b)
{
  double c = (25*pow(b, 9))/9 - 2*pow(b, 5) +b;
  double d = (25*pow(a, 9))/9 - 2*pow(a, 5) +a;

  double e = c - d;

  return e/n;
}

/*
    Integration de Monte-Carlo
*/
double int_mc(double(*f)(double), double& varest, double a, double b, int n)
{

  std::uniform_real_distribution<double> unif(a, b);
  std::random_device rand_dev;
  std::mt19937 rand_engine(rand_dev());
  double randomNumber;

  double ret = 0.f;
  double sumVar1 = 0.f;
  double sumVar2 = 0.f;

  for (int i = 0; i < n; i++) {
    randomNumber = unif(rand_engine);
    ret += f(randomNumber);

    randomNumber = unif(rand_engine);
    sumVar1 += pow(f(randomNumber),2);

    randomNumber = unif(rand_engine);
    sumVar2 += f(randomNumber);
  }

  sumVar1 /= n;
  sumVar2 = pow(sumVar2/n, 2);
  varest = (sumVar1 - sumVar2)/(n-1);

  return ret/n;
}

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}
