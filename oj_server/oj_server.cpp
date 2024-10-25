#include <csignal>

#include "../include/httplib.h"
#include "oj_controller.h"

oj_controller::controller* ctrl_ptr;
void online(int) { ctrl_ptr->online_service(); }

int main() {
  httplib::Server svr;

  oj_controller::controller ctrl;
  ctrl_ptr = &ctrl;
  signal(SIGQUIT, online); /* Ctrl+\ */

  svr.Get("/problemset",
          [&](const httplib::Request& rqst, httplib::Response& rsps) {
            auto html = ctrl.get_problemset();
            rsps.set_content(html, "text/html; charset=utf-8");
          });

  svr.Get(R"(/problem/(\d+))",
          [&](const httplib::Request& rqst, httplib::Response& rsps) {
            std::string id = rqst.matches[1];
            auto html = ctrl.get_problem(id);
            rsps.set_content(html, "text/html; charset=utf-8");
          });

  svr.Post(R"(/judge/(\d+))",
           [&](const httplib::Request& rqst, httplib::Response& rsps) {
             std::string id = rqst.matches[1];
             auto res_json = ctrl.judge(id, rqst.body);
             rsps.set_content(res_json, "application/json; charset=utf-8");
           });

  svr.set_base_dir("./root");
  svr.listen("0.0.0.0", 8080);
}