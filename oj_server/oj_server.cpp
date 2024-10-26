#include <csignal>

#include "../include/httplib.h"
#include "oj_controller.h"

oj_controller::controller* pctrl;
void online(int) { pctrl->online_service(); }

int main() {
  httplib::Server svr;

  oj_controller::controller ctrl;
  pctrl = &ctrl;

  //click 'Ctrl+\' to online
  signal(SIGQUIT, online);

  svr.set_base_dir("./root");

  svr.Get("/problemset",
          [&ctrl](const httplib::Request& rq, httplib::Response& rp) {
            auto html = ctrl.get_problemset();
            rp.set_content(html, "text/html; charset=utf-8");
          });

  svr.Get(R"(/problem/(\d+))",
          [&ctrl](const httplib::Request& rq, httplib::Response& rp) {
            std::string id = rq.matches[1];
            auto html = ctrl.get_problem(id);
            rp.set_content(html, "text/html; charset=utf-8");
          });

  svr.Post(R"(/judge/(\d+))",
           [&ctrl](const httplib::Request& rq, httplib::Response& rp) {
             std::string id = rq.matches[1];
             auto res_json = ctrl.judge(id, rq.body);
             rp.set_content(res_json, "application/json; charset=utf-8");
           });

  svr.listen("0.0.0.0", 8080);
}