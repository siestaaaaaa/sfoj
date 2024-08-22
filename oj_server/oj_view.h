#pragma once

#include <ctemplate/template.h>

#include "oj_model.h"

namespace oj_view {
using namespace oj_model;

class view {
  static inline const std::string template_html_path = "./template_html/";

 public:
  void expand(const problem& problem, std::string& output) {
    ctemplate::TemplateDictionary root("problem");
    root.SetValue("id", problem.id_);
    root.SetValue("name", problem.name_);
    root.SetValue("level", problem.level_);
    root.SetValue("description", problem.description_);
    root.SetValue("solution", problem.solution_);

    auto path = template_html_path + "problem.html";
    auto templ =
        ctemplate::Template::GetTemplate(path, ctemplate::DO_NOT_STRIP);
    templ->Expand(&output, &root);
  }

  void expand(const std::vector<problem>& problemset, std::string& output) {
    ctemplate::TemplateDictionary root("problemset");
    for (const auto& problem : problemset) {
      auto t = root.AddSectionDictionary("problem_list");
      t->SetValue("id", problem.id_);
      t->SetValue("name", problem.name_);
      t->SetValue("level", problem.level_);
    }

    auto path = template_html_path + "problemset.html";
    auto templ =
        ctemplate::Template::GetTemplate(path, ctemplate::DO_NOT_STRIP);
    templ->Expand(&output, &root);
  }
};
}  // namespace oj_view