#include "PID.h"

using namespace std;


PID::PID(bool p_enable, bool i_enable, bool d_enable) {
	this->p_enabled = p_enable;
	this->i_enabled = i_enable;
	this->d_enabled = d_enable;

	default_random_engine generator;
	uniform_real_distribution<double> kp_dist(0.4, 0.7);
	uniform_real_distribution<double> ki_dist(0.0, 0.4);
	uniform_real_distribution<double> kd_dist(0.7, 5.0);

	this->Kp = kp_dist(generator);
	this->Ki = ki_dist(generator);
	this->Kd = kd_dist(generator);

	this->epoch = -1;

	this->alpha = 1.1;
	this->epsilon = 1e-07;

	this->output = 0.0;
}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;

	this->p_error = 0.0;
	this->i_error = 0.0;
	this->d_error = 0.0;
}

void PID::Update(double error) {

	this->epoch += 1;

	if (this->epoch == 0) {
		this->p_error = error;
		return;
	}

	double p_tm1 = this->p_error;
	double i_tm1 = this->i_error;
	double d_tm1 = this->d_error;

		// update errors
	this->p_error = error;
	this->i_error += error;
	this->d_error = (error - p_tm1);

	if (error > epsilon) {
			// apply gradient descent
		double kp_d = (this->Kp * (this->p_error - p_tm1)) * error;
		double ki_d = (this->Ki * (this->i_error - i_tm1)) * error;
		double kd_d = (this->Kd * (this->d_error - d_tm1)) * error;

		this->Kp = this->Kp - (this->alpha * kp_d);
		this->Ki = this->Ki - (this->alpha * ki_d);
		this->Kd = this->Kd - (this->alpha * kd_d);
	}
}

double PID::Compute() {

	this->output = ((-this->Kp * this->p_error) * this->p_enabled) // Proportional gain
								- ((this->Kd * this->d_error) * this->d_enabled) // Differential gain
								- ((this->Ki * this->i_error) * this->i_enabled); // Integral gain

	return this->output;
}

double PID::TotalError() {
	// compute sum of errors
	double sum = this->d_error + this->i_error + this->p_error;

	return sum;
}

