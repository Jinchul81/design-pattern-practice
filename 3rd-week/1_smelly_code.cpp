#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

using namespace std;

using AttributeMap = unordered_map<string, string>;
class HtmlTag {
  string name;
  AttributeMap attributes;
  bool is_singular;
  string text;
  vector<HtmlTag> children;

  static constexpr auto INDENT_SIZE = 2u;
  static string gen_indent(size_t level) {
    return string(level*INDENT_SIZE, ' ');
  }
  ostream& gen(size_t level, ostream& o) const {
    const string indent = gen_indent(level);
    o << indent << "<" << name;
    for (auto [key, value]:attributes)
      o << " " << key << R"(=")" << value << R"(")";
    o << ">" << endl;
    if (! is_singular) {
      if (text.size())
        o << gen_indent(level+1) << text << endl;
      for (auto& child:children)
        child.gen(level+1, o);
      o << indent << "</" << name << ">" << endl;
    }
    return o;
  }

public:
  HtmlTag(string_view name,
          AttributeMap&& attributes,
          bool is_singular = false)
    : name(name), attributes(move(attributes))
    , is_singular(is_singular) {
  }

  HtmlTag(string_view name,
          bool is_singular = false)
    : HtmlTag(name, {}, is_singular) {
  }

  void add(HtmlTag&& tag) {
    children.push_back(move(tag));
  }

  void set_text(string_view t) {
    if (is_singular)
      throw runtime_error(
        "Singular tag does not support text message");
    text = t;
  }

  string str() const {
    ostringstream oss;
    gen(0, oss);
    return oss.str();
  }
};

int main() {
  HtmlTag html("html");
  {
    HtmlTag head("head");
    {
      HtmlTag title("title");
      title.set_text("Title of the document");
      head.add(move(title));
    } // title
    html.add(move(head));
  } // head
  {
    HtmlTag body("body");
    {
      HtmlTag table("table",
        {{"cellspacing", "0"},
          {"summay", "Color names for black, white, red..."},
          {"class", "chart"}});
      {
        HtmlTag caption("caption");
        caption.set_text("Color Names in Multiple Languages");
        table.add(move(caption));
      } // caption
      {
        HtmlTag thead("thead");
        {
          HtmlTag tr("tr");
          {
            HtmlTag th("th", {{"id", "fam"},
                              {"class", "toplevel"}});
            th.set_text("Family");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"id", "rmc"},
                              {"class", "toplevel"},
                              {"colspan", "2"}});
            th.set_text("Romance Lang");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"id", "cel"},
                              {"class", "toplevel"},
                              {"colspan", "2"}});
            th.set_text("Celtic");
            tr.add(move(th));
          } // th
          thead.add(move(tr));
        } // tr
        {
          HtmlTag tr("tr");
          {
            HtmlTag th("th", {{"headers", "hue"}});
            th.set_text("Color");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"headers", "rmc"},
                              {"id", "es"}});
            th.set_text("Spanish");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"headers", "rmc"},
                              {"id", "fr"}});
            th.set_text("French");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"headers", "rmc"},
                              {"id", "ga"}});
            th.set_text("Irish");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"headers", "rmc"},
                              {"id", "cy"}});
            th.set_text("Welsh");
            tr.add(move(th));
          } // th
          thead.add(move(tr));
        } // tr
        table.add(move(thead));
      } // thead
      {
        HtmlTag tbody("tbody");
        {
          HtmlTag tr("tr");
          {
            HtmlTag th("th", {{"headers", "hue"},
                {"id", "green"}});
            th.set_text("Green");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"lang", "es"},
                {"headers", "rms es green"}});
            th.set_text("verde");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"lang", "fr"},
                {"headers", "rms fr green"}});
            th.set_text("vert");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"lang", "ga"},
                {"headers", "cel ga green"}});
            th.set_text("glas");
            tr.add(move(th));
          } // th
          {
            HtmlTag th("th", {{"lang", "cy"},
                {"headers", "cel cy green"}});
            th.set_text("gwyrdd");
            tr.add(move(th));
          } // th
          tbody.add(move(tr));
        } // tr
        table.add(move(tbody));
      } // tbody
      {
        HtmlTag hr("hr", true);
        table.add(move(hr));
      } // hr
      {
        HtmlTag a("a", {{"target", "_blank"},
                        {"href", "https://foo.bar"}});
        a.set_text("Please click me!!!!");
        table.add(move(a));
      } // a
      body.add(move(table));
    } // table
    html.add(move(body));
  } // body

  cout << html.str() << endl;

  return 0;
}
