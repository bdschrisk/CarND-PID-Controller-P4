#define _USE_MATH_DEFINES

#include <uWS/uWS.h>
#include <iostream>
#include <math.h>
#include "json.hpp"

#include "PID.h"

using namespace std;

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }
double clip(double v, double low, double high) { return max(low, min(v, high)); }

const double max_speed = 100.0;
// Recovery driving factor (0..1)
const double recover_factor = 0.4;
// Smoothing factor (0..1)
const double scale_factor = 0.4;

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
stringstream hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
		return stringstream();
  }
	else if (b1 != string::npos && b2 != string::npos) {
		stringstream tmp = stringstream();
		tmp.str(s.substr(b1, b2 - b1 + 1));
		return tmp;
	}
	return stringstream();
}

int main()
{
  uWS::Hub h;

  PID pid_steering;
	PID pid_gain;

	h.onMessage([&pid_steering, &pid_gain](uWS::WebSocket<uWS::SERVER> *ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(string(data));
      if (s.str() != "") {
        auto j = json::parse(s);
        string event = j[0].get<string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = stod(j[1]["cte"].get<string>());
          double speed = stod(j[1]["speed"].get<string>());
          double angle = stod(j[1]["steering_angle"].get<string>());

					angle = deg2rad(angle);

					double p_tm1 = pid_gain.p_error;

					pid_gain.Update(cte * (1.0-recover_factor));
					double val = pid_gain.Compute();
					
					pid_steering.Update(-val * recover_factor);
					double steer_value = pid_steering.Compute();

					cout << "Regulator: " << steer_value << "  |  Kp: " << pid_steering.Kp << " Ki: " << pid_steering.Ki << " Kd: " << pid_steering.Kd << endl;
					cout << "Steering: " << val << "  |  Kp: " << pid_gain.Kp << " Ki: " << pid_gain.Ki << " Kd: " << pid_gain.Kd << endl;

					steer_value = (steer_value * recover_factor) + (val * (1.0-recover_factor));
					steer_value = (steer_value * scale_factor) + (angle * (1.0-scale_factor));
					steer_value = clip(steer_value, -1.0, 1.0);
					
					// stop learning after 1200 epochs
					if (pid_steering.epoch == 1200) {
						pid_steering.Stop();
						pid_gain.Stop();
						cout << "\nOptimised!\n" << endl;
					}

          // DEBUG
          cout << "CTE: " << cte << "  Steering: " << steer_value << "  Speed: " << speed << endl;

					json msgJson;
					msgJson["steering_angle"] = steer_value;
					msgJson["throttle"] = 0.48;
					
					auto msg = "42[\"steer\"," + msgJson.dump() + "]";
					
					(*ws).send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
				// Manual driving
				std::string msg = "42[\"manual\",{}]";
				(*ws).send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

	h.onConnection([&h](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
		std::cout << "Connected!!!" << std::endl;
	});

	h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
		(*ws).close();
		std::cout << "Disconnected" << std::endl;
	});

	int port = 4567;
	if (h.listen("0.0.0.0", port))
	{
		std::cout << "Listening to port " << port << std::endl;
	}
	else
	{
		std::cerr << "Failed to listen to port" << std::endl;
		return -1;
	}
	h.run();
}
