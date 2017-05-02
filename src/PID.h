#ifndef PID_H
#define PID_H

#include <random>
#include <numeric>
#include <math.h>
#include <float.h>

class PID {
private:
	bool p_enabled;
	bool i_enabled;
	bool d_enabled;
	bool enabled;

public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

	double output;
	
	int epoch;
	double alpha;
	double epsilon;

  /*
  * Constructor
  */
  PID(bool p_enable = true, bool i_enable = true, bool d_enable = true);

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given an error signal.
  */
  void Update(double error);

	/*
	* Stops learning
	*/
	void Stop();

	/*
	* Computes the output of the PID controller
	*/
	double Compute();

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */
