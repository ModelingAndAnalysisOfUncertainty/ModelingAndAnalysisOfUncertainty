#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "QuadProg++.hh"



int main(){
    quadprogpp::Matrix<double> G;
    G.resize(2,2);
    G[0][0] = 2;
    G[0][1] = 0;
    G[1][0] = 0;
    G[1][1] = 4;


    quadprogpp::Vector<double> g0;
    g0.resize(2);
    g0[0] = -4;
    g0[1] = -8;

 
    quadprogpp::Matrix<double> CE;
    CE.resize(2,2); // 2 row, 1 column
    CE[0][0] = 1;
    CE[0][1] = 1;
    CE[1][0] = -1;
    // CE[1][1] = 0;
    // CE[2][0] = 0;
  

  
    quadprogpp::Vector<double> ce0;
    ce0.resize(2);
    ce0[0] = 3;
    ce0[1] = 0;
    // ce0[2] = 0;

    quadprogpp::Matrix<double> CI;
    CI.resize(2,1);
    CI[0][0] = 0;
    CI[0][1] = 0;
    CI[1][0] = 0;
    CI[1][1] = 0;


    quadprogpp::Vector<double> ci0;
    ci0.resize(1);
    ci0[0] = 0;
    // ci0[1] = 0;

    quadprogpp::Vector<double> x;
    x.resize(2);


    // try {
        double result = solve_quadprog(G, g0, CE, ce0, CI, ci0, x);

  
        std::cout << "Optimal value: " << result << std::endl;
        std::cout << "Optimal solution (x): ";
        for (int i = 0; i < x.size(); i++) {
            std::cout << x[i] << " ";
        }
        std::cout << std::endl;
    // } 
    // catch (const std::exception &e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    // }

    return 0;
}

// double solve_quadprog(Matrix<double>& G, Vector<double>& g0, 
//                       const Matrix<double>& CE, const Vector<double>& ce0,  
//                       const Matrix<double>& CI, const Vector<double>& ci0, 
//                       Vector<double>& x)
// {
//   std::ostringstream msg;
//   unsigned int n = G.ncols(), p = CE.ncols(), m = CI.ncols();
//   if (G.nrows() != n)
//   {
//     msg << "The matrix G is not a squared matrix (" << G.nrows() << " x " << G.ncols() << ")";
//     throw std::logic_error(msg.str());
//   }
//   if (CE.nrows() != n)
//   {
//     msg << "The matrix CE is incompatible (incorrect number of rows " << CE.nrows() << " , expecting " << n << ")";
//     throw std::logic_error(msg.str());
//   }
//   if (ce0.size() != p)
//   {
//     msg << "The vector ce0 is incompatible (incorrect dimension " << ce0.size() << ", expecting " << p << ")";
//     throw std::logic_error(msg.str());
//   }
//   if (CI.nrows() != n)
//   {
//     msg << "The matrix CI is incompatible (incorrect number of rows " << CI.nrows() << " , expecting " << n << ")";
//     throw std::logic_error(msg.str());
//   }
//   if (ci0.size() != m)
//   {
//     msg << "The vector ci0 is incompatible (incorrect dimension " << ci0.size() << ", expecting " << m << ")";
//     throw std::logic_error(msg.str());
//   }
//   x.resize(n);
//   unsigned int i, j, k, l; /* indices */
//   int ip; // this is the index of the constraint to be added to the active set
//   Matrix<double> R(n, n), J(n, n);
//   Vector<double> s(m + p), z(n), r(m + p), d(n), np(n), u(m + p), x_old(n), u_old(m + p);
//   double f_value, psi, c1, c2, sum, ss, R_norm;
//   double inf;
//   if (std::numeric_limits<double>::has_infinity)
//     inf = std::numeric_limits<double>::infinity();
//   else
//     inf = 1.0E300;
//   double t, t1, t2; /* t is the step lenght, which is the minimum of the partial step length t1 
//     * and the full step length t2 */
//   Vector<int> A(m + p), A_old(m + p), iai(m + p);
//   unsigned int iq, iter = 0;
//   Vector<bool> iaexcl(m + p);
	
//   /* p is the number of equality constraints */
//   /* m is the number of inequality constraints */
// #ifdef TRACE_SOLVER
//   std::cout << std::endl << "Starting solve_quadprog" << std::endl;
//   print_matrix("G", G);
//   print_vector("g0", g0);
//   print_matrix("CE", CE);
//   print_vector("ce0", ce0);
//   print_matrix("CI", CI);
//   print_vector("ci0", ci0);
// #endif  
  
//   /*
//    * Preprocessing phase
//    */
	
//   /* compute the trace of the original matrix G */
//   c1 = 0.0;
//   for (i = 0; i < n; i++)
//   {
//     c1 += G[i][i];
//   }
//   /* decompose the matrix G in the form L^T L */
//   cholesky_decomposition(G);
// #ifdef TRACE_SOLVER
//   print_matrix("G", G);
// #endif
//   /* initialize the matrix R */
//   for (i = 0; i < n; i++)
//   {
//     d[i] = 0.0;
//     for (j = 0; j < n; j++)
//       R[i][j] = 0.0;
//   }
//   R_norm = 1.0; /* this variable will hold the norm of the matrix R */
  
//   /* compute the inverse of the factorized matrix G^-1, this is the initial value for H */
//   c2 = 0.0;
//   for (i = 0; i < n; i++) 
//   {
//     d[i] = 1.0;
//     forward_elimination(G, z, d);
//     for (j = 0; j < n; j++)
//       J[i][j] = z[j];
//     c2 += z[i];
//     d[i] = 0.0;
//   }
// #ifdef TRACE_SOLVER
//   print_matrix("J", J);
// #endif
  
//   /* c1 * c2 is an estimate for cond(G) */
  
//   /* 
//     * Find the unconstrained minimizer of the quadratic form 0.5 * x G x + g0 x 
//    * this is a feasible point in the dual space
//    * x = G^-1 * g0
//    */
//   cholesky_solve(G, x, g0);
//   for (i = 0; i < n; i++)
//     x[i] = -x[i];
//   /* and compute the current solution value */ 
//   f_value = 0.5 * scalar_product(g0, x);
// #ifdef TRACE_SOLVER
//   std::cout << "Unconstrained solution: " << f_value << std::endl;
//   print_vector("x", x);
// #endif
  
//   /* Add equality constraints to the working set A */
//   iq = 0;
//   for (i = 0; i < p; i++)
//   {
//     for (j = 0; j < n; j++)
//       np[j] = CE[j][i];
//     compute_d(d, J, np);
//     update_z(z, J, d, iq);
//     update_r(R, r, d, iq);
// #ifdef TRACE_SOLVER
//     print_matrix("R", R, n, iq);
//     print_vector("z", z);
//     print_vector("r", r, iq);
//     print_vector("d", d);
// #endif
    
//     /* compute full step length t2: i.e., the minimum step in primal space s.t. the contraint 
//       becomes feasible */
//     t2 = 0.0;
//     if (fabs(scalar_product(z, z)) > std::numeric_limits<double>::epsilon()) // i.e. z != 0
//       t2 = (-scalar_product(np, x) - ce0[i]) / scalar_product(z, np);
    
//     /* set x = x + t2 * z */
//     for (k = 0; k < n; k++)
//       x[k] += t2 * z[k];
    
//     /* set u = u+ */
//     u[iq] = t2;
//     for (k = 0; k < iq; k++)
//       u[k] -= t2 * r[k];
    
//     /* compute the new solution value */
//     f_value += 0.5 * (t2 * t2) * scalar_product(z, np);
//     A[i] = -i - 1;
    
//     if (!add_constraint(R, J, d, iq, R_norm))
//     {	  
//       // Equality constraints are linearly dependent
//       throw std::runtime_error("Constraints are linearly dependent");
//       return f_value;
//     }
//   }
  
//   /* set iai = K \ A */
//   for (i = 0; i < m; i++)
//     iai[i] = i;
  
// l1:	iter++;
// #ifdef TRACE_SOLVER
//   print_vector("x", x);
// #endif
//   /* step 1: choose a violated constraint */
//   for (i = p; i < iq; i++)
//   {
//     ip = A[i];
//     iai[ip] = -1;
//   }
	
//   /* compute s[x] = ci^T * x + ci0 for all elements of K \ A */
//   ss = 0.0;
//   psi = 0.0; /* this value will contain the sum of all infeasibilities */
//   ip = 0; /* ip will be the index of the chosen violated constraint */
//   for (i = 0; i < m; i++)
//   {
//     iaexcl[i] = true;
//     sum = 0.0;
//     for (j = 0; j < n; j++)
//       sum += CI[j][i] * x[j];
//     sum += ci0[i];
//     s[i] = sum;
//     psi += std::min(0.0, sum);
//   }
// #ifdef TRACE_SOLVER
//   print_vector("s", s, m);
// #endif
  
  
//   if (fabs(psi) <= m * std::numeric_limits<double>::epsilon() * c1 * c2* 100.0)
//   {
//     /* numerically there are not infeasibilities anymore */
//     return f_value;
//   }
  
//   /* save old values for u and A */
//   for (i = 0; i < iq; i++)
//   {
//     u_old[i] = u[i];
//     A_old[i] = A[i];
//   }
//   /* and for x */
//   for (i = 0; i < n; i++)
//     x_old[i] = x[i];
  
// l2: /* Step 2: check for feasibility and determine a new S-pair */
//     for (i = 0; i < m; i++)
//     {
//       if (s[i] < ss && iai[i] != -1 && iaexcl[i])
//       {
//         ss = s[i];
//         ip = i;
//       }
//     }
//   if (ss >= 0.0)
//   {
//     return f_value;
//   }
  
//   /* set np = n[ip] */
//   for (i = 0; i < n; i++)
//     np[i] = CI[i][ip];
//   /* set u = [u 0]^T */
//   u[iq] = 0.0;
//   /* add ip to the active set A */
//   A[iq] = ip;
  
// #ifdef TRACE_SOLVER
//   std::cout << "Trying with constraint " << ip << std::endl;
//   print_vector("np", np);
// #endif
  
// l2a:/* Step 2a: determine step direction */
//     /* compute z = H np: the step direction in the primal space (through J, see the paper) */
//     compute_d(d, J, np);
//   update_z(z, J, d, iq);
//   /* compute N* np (if q > 0): the negative of the step direction in the dual space */
//   update_r(R, r, d, iq);
// #ifdef TRACE_SOLVER
//   std::cout << "Step direction z" << std::endl;
//   print_vector("z", z);
//   print_vector("r", r, iq + 1);
//   print_vector("u", u, iq + 1);
//   print_vector("d", d);
//   print_vector("A", A, iq + 1);
// #endif
  
//   /* Step 2b: compute step length */
//   l = 0;
//   /* Compute t1: partial step length (maximum step in dual space without violating dual feasibility */
//   t1 = inf; /* +inf */
//   /* find the index l s.t. it reaches the minimum of u+[x] / r */
//   for (k = p; k < iq; k++)
//   {
//     if (r[k] > 0.0)
//     {
//       if (u[k] / r[k] < t1)
// 	    {
// 	      t1 = u[k] / r[k];
// 	      l = A[k];
// 	    }
//     }
//   }
//   /* Compute t2: full step length (minimum step in primal space such that the constraint ip becomes feasible */
//   if (fabs(scalar_product(z, z))  > std::numeric_limits<double>::epsilon()) // i.e. z != 0
//   {
//     t2 = -s[ip] / scalar_product(z, np);
//     if (t2 < 0) // patch suggested by Takano Akio for handling numerical inconsistencies
//       t2 = inf;
//   }
//   else
//     t2 = inf; /* +inf */
  
//   /* the step is chosen as the minimum of t1 and t2 */
//   t = std::min(t1, t2);
// #ifdef TRACE_SOLVER
//   std::cout << "Step sizes: " << t << " (t1 = " << t1 << ", t2 = " << t2 << ") ";
// #endif
  
//   /* Step 2c: determine new S-pair and take step: */
  
//   /* case (i): no step in primal or dual space */
//   if (t >= inf)
//   {
//     /* QPP is infeasible */
//     // FIXME: unbounded to raise
//     return inf;
//   }
//   /* case (ii): step in dual space */
//   if (t2 >= inf)
//   {
//     /* set u = u +  t * [-r 1] and drop constraint l from the active set A */
//     for (k = 0; k < iq; k++)
//       u[k] -= t * r[k];
//     u[iq] += t;
//     iai[l] = l;
//     delete_constraint(R, J, A, u, n, p, iq, l);
// #ifdef TRACE_SOLVER
//     std::cout << " in dual space: " 
//       << f_value << std::endl;
//     print_vector("x", x);
//     print_vector("z", z);
//     print_vector("A", A, iq + 1);
// #endif
//     goto l2a;
//   }
  
//   /* case (iii): step in primal and dual space */
  
//   /* set x = x + t * z */
//   for (k = 0; k < n; k++)
//     x[k] += t * z[k];
//   /* update the solution value */
//   f_value += t * scalar_product(z, np) * (0.5 * t + u[iq]);
//   /* u = u + t * [-r 1] */
//   for (k = 0; k < iq; k++)
//     u[k] -= t * r[k];
//   u[iq] += t;
// #ifdef TRACE_SOLVER
//   std::cout << " in both spaces: " 
//     << f_value << std::endl;
//   print_vector("x", x);
//   print_vector("u", u, iq + 1);
//   print_vector("r", r, iq + 1);
//   print_vector("A", A, iq + 1);
// #endif
  
//   if (fabs(t - t2) < std::numeric_limits<double>::epsilon())
//   {
// #ifdef TRACE_SOLVER
//     std::cout << "Full step has taken " << t << std::endl;
//     print_vector("x", x);
// #endif
//     /* full step has taken */
//     /* add constraint ip to the active set*/
//     if (!add_constraint(R, J, d, iq, R_norm))
//     {
//       iaexcl[ip] = false;
//       delete_constraint(R, J, A, u, n, p, iq, ip);
// #ifdef TRACE_SOLVER
//       print_matrix("R", R);
//       print_vector("A", A, iq);
// 			print_vector("iai", iai);
// #endif
//       for (i = 0; i < m; i++)
//         iai[i] = i;
//       for (i = p; i < iq; i++)
// 	    {
// 	      A[i] = A_old[i];
// 	      u[i] = u_old[i];
// 				iai[A[i]] = -1;
// 	    }
//       for (i = 0; i < n; i++)
//         x[i] = x_old[i];
//       goto l2; /* go to step 2 */
//     }    
//     else
//       iai[ip] = -1;
// #ifdef TRACE_SOLVER
//     print_matrix("R", R);
//     print_vector("A", A, iq);
// 		print_vector("iai", iai);
// #endif
//     goto l1;
//   }
  
//   /* a patial step has taken */
// #ifdef TRACE_SOLVER
//   std::cout << "Partial step has taken " << t << std::endl;
//   print_vector("x", x);
// #endif
//   /* drop constraint l */
//   iai[l] = l;
//   delete_constraint(R, J, A, u, n, p, iq, l);
// #ifdef TRACE_SOLVER
//   print_matrix("R", R);
//   print_vector("A", A, iq);
// #endif
  
//   /* update s[ip] = CI * x + ci0 */
//   sum = 0.0;
//   for (k = 0; k < n; k++)
//     sum += CI[k][ip] * x[k];
//   s[ip] = sum + ci0[ip];
  
// #ifdef TRACE_SOLVER
//   print_vector("s", s, m);
// #endif
//   goto l2a;
// }