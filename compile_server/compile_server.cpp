#include "../include/httplib.h"
#include "compile_run.h"

using namespace compile_run;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Syntax:\n\t" << argv[0] << " port\n";
    return 1;
  }

  httplib::Server svr;
  svr.Post("/compile_run",
           [](const httplib::Request& rq, httplib::Response& rp) {
             auto in_json = rq.body;
             if (!in_json.empty()) {
               auto out_json = compile_and_run::start(in_json);
               rp.set_content(out_json, "application/json; charset=utf-8");
             }
           });
  svr.listen("0.0.0.0", atoi(argv[1]));
}