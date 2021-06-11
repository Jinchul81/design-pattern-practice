#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <unordered_map>

using namespace std;

struct IHtmlTag {
  static constexpr auto INDENT_SIZE = 2u;
  virtual ostream& gen(size_t level, ostream& o) const = 0;
  string str() const {
    ostringstream oss;
    gen(0, oss);
    return oss.str();
  }
  static string gen_indent(size_t level) {
    return string(level*INDENT_SIZE, ' ');
  }
};

using IHtmlTagUPtr = unique_ptr<IHtmlTag>;
using IHtmlTagUPtrList = initializer_list<IHtmlTagUPtr>;
using AttributeMap = unordered_map<string, string>;

class PlainText final : public IHtmlTag {
  string text;

protected:
  virtual ostream& gen(size_t level, ostream& o) const {
    const string indent = gen_indent(level);
    o << indent << text << endl;
    return o;
  }

  PlainText(string_view text)
    : text(text) {
  }

public:
  virtual ~PlainText() = default;

  static IHtmlTagUPtr create(string_view text) {
    return IHtmlTagUPtr(new PlainText(text));
  }
};

class HtmlTag : public IHtmlTag {
protected:
  string name;
  AttributeMap attributes;

  ostream& gen_open_tag(ostream& o) const {
    o << "<" << name;
    for (auto [key, value]:attributes)
      o << " " << key << R"(=")" << value << R"(")";
    o << ">";
    return o;
  }

  ostream& gen_close_tag(ostream& o) const {
    o << "</" << name << ">";
    return o;
  }

public:
  HtmlTag(string_view name)
    : name(name) {
  }
  HtmlTag(string_view name, AttributeMap&& attributes)
    : name(name), attributes(move(attributes)) {
  }
  virtual ~HtmlTag() = default;
};

template <typename T>
class SingularTag : public HtmlTag {
protected:
  virtual ostream& gen(size_t level, ostream& o) const {
    const string indent = gen_indent(level);
    o << indent; gen_open_tag(o); o << endl;
    return o;
  }

  SingularTag(string_view name = T::name)
    : HtmlTag(name) {
  }
  SingularTag(AttributeMap&& attributes, string_view = T::name)
    : HtmlTag(name, move(attributes)) {
  }

public:
  virtual ~SingularTag() = default;
  static IHtmlTagUPtr create() {
    return IHtmlTagUPtr(new SingularTag<T>());
  }
  static IHtmlTagUPtr create(AttributeMap&& attributes) {
    return IHtmlTagUPtr(new SingularTag<T>(move(attributes)));
  }
};

template <typename T>
class PairedTag : public HtmlTag {
  vector<IHtmlTagUPtr> children;

  void add_children(IHtmlTagUPtrList& list) {
    for (auto itr = list.begin(); itr != list.end(); ++itr)
      children.push_back(move(*const_cast<IHtmlTagUPtr*>(itr)));
  }

protected:
  virtual ostream& gen(size_t level, ostream& o) const {
    const string indent = gen_indent(level);
    o << indent; gen_open_tag(o); o << endl;
    for (auto& child:children)
      child->gen(level+1, o);
    o << indent; gen_close_tag(o); o << endl;
    return o;
  }

  PairedTag(string_view name = T::name)
    : HtmlTag(name) {
  }
  PairedTag(IHtmlTagUPtrList& list, string_view name = T::name)
    : HtmlTag(name) {
      add_children(list);
  }
  PairedTag(IHtmlTagUPtrList list,
            AttributeMap&& attributes,
            string_view name = T::name)
    : HtmlTag(name, move(attributes)) {
      add_children(list);
  }

public:
  virtual ~PairedTag() = default;
  static IHtmlTagUPtr create() {
    return IHtmlTagUPtr(new PairedTag<T>());
  }
  static IHtmlTagUPtr create(IHtmlTagUPtrList list) {
    return IHtmlTagUPtr(new PairedTag<T>(list));
  }
  static IHtmlTagUPtr create(IHtmlTagUPtrList list,
                             AttributeMap&& attributes) {
    return IHtmlTagUPtr(new PairedTag<T>(list, move(attributes)));
  }
};

// Specialized for PairedTag
struct TagHtmlInternal {
  static constexpr auto name = "html"sv;
};
using TagHtml = PairedTag<TagHtmlInternal>;

struct TagHeadInternal {
  static constexpr auto name = "head"sv;
};
using TagHead = PairedTag<TagHeadInternal>;

struct TagTitleInternal {
  static constexpr auto name = "title"sv;
};
using TagTitle = PairedTag<TagTitleInternal>;

struct TagBodyInternal {
  static constexpr auto name = "body"sv;
};
using TagBody = PairedTag<TagBodyInternal>;

struct TagTableInternal {
  static constexpr auto name = "table"sv;
};
using TagTable = PairedTag<TagTableInternal>;

struct TagTheadInternal {
  static constexpr auto name = "thead"sv;
};
using TagThead = PairedTag<TagTheadInternal>;

struct TagTbodyInternal {
  static constexpr auto name = "tbody"sv;
};
using TagTbody = PairedTag<TagTbodyInternal>;

struct TagTrInternal {
  static constexpr auto name = "tr"sv;
};
using TagTr = PairedTag<TagTrInternal>;

struct TagThInternal {
  static constexpr auto name = "th"sv;
};
using TagTh = PairedTag<TagThInternal>;

struct TagCaptionInternal {
  static constexpr auto name = "caption"sv;
};
using TagCaption = PairedTag<TagCaptionInternal>;

struct TagAInternal {
  static constexpr auto name = "a"sv;
};
using TagA = PairedTag<TagAInternal>;

// Specialized for SingularTag
struct TagBrInternal {
  static constexpr auto name = "br"sv;
};
using TagBr = SingularTag<TagBrInternal>;

struct TagHrInternal {
  static constexpr auto name = "hr"sv;
};
using TagHr = SingularTag<TagHrInternal>;

void test() {
  IHtmlTagUPtr root =
    TagHtml::create({
      TagHead::create({
        TagTitle::create({
          PlainText::create("Title of the document")
        }) // title
      }), // head
      TagBody::create({
        TagTable::create({
          TagCaption::create({
            PlainText::create(
              "Color Names in Multiple Languages")
          }), // caption
          TagThead::create({
            TagTr::create({
              TagTh::create({
                PlainText::create("Family")
                }, {{"id", "fam"},
                    {"class", "toplevel"}
              }), // th
              TagTh::create({
                PlainText::create("Romance Lang")
                }, {{"id", "rmc"},
                    {"class", "toplevel"},
                    {"colspan", "2"}
              }), // th
              TagTh::create({
                PlainText::create("Celtic")
                }, {{"id", "cel"},
                    {"class", "toplevel"},
                    {"colspan", "2"}
              }) // th
            }), // tr
            TagTr::create({
              TagTh::create({
                PlainText::create("Color")
                }, {{"headers", "hue"},
              }), // th
              TagTh::create({
                PlainText::create("Spanish")
                }, {{"headers", "rmc"},
                    {"id", "es"},
              }), // th
              TagTh::create({
                PlainText::create("French")
                }, {{"headers", "rmc"},
                    {"id", "fr"},
              }), // th
              TagTh::create({
                PlainText::create("Irish")
                }, {{"headers", "rmc"},
                    {"id", "ga"},
              }), // th
              TagTh::create({
                PlainText::create("Welsh")
                }, {{"headers", "rmc"},
                    {"id", "cy"},
              }) // th
            }) // tr
          }), // thead
          TagTbody::create({
            TagTr::create({
              TagTh::create({
                PlainText::create("Green")
                }, {{"headers", "hue"},
                    {"id", "green"}
              }), // th
              TagTh::create({
                PlainText::create("verde")
                }, {{"lang", "es"},
                    {"headers", "rms es green"}
              }), // th
              TagTh::create({
                PlainText::create("vert")
                }, {{"lang", "fr"},
                    {"headers", "rms fr green"}
              }), // th
              TagTh::create({
                PlainText::create("glas")
                }, {{"lang", "ga"},
                    {"headers", "cel ga green"}
              }), // th
              TagTh::create({
                PlainText::create("gwyrdd")
                }, {{"lang", "cy"},
                    {"headers", "cel cy green"}
              }) // th
            }) // tr
          }) // tbody
        }, {{"cellspacing", "0"},
            {"summay", "Color names for black, white, red..."},
            {"class", "chart"}
        }), // table
        TagHr::create(),
        TagA::create({
          PlainText::create("Please"),
          TagBr::create(),
          PlainText::create("click"),
          TagBr::create(),
          PlainText::create("me!!!!")},
        {{"target", "_blank"},
         {"href", "https://foo.bar"}
       }) // A
      }) // body
    }, {{"lang", "en"}}); // html
  cout << root->str() << endl;
}

int main() {
  test();

  return 0;
}
