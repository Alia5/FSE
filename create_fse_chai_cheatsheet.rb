require "fileutils"

def main()
  create_cheatsheet()
end

class CheatsheetClass
  def initialize(name, section_name)
    @name = name
    @section_name = section_name
    @funcs = Array.new
    @ctors = Array.new
    @parent = nil
  end

  def name
    @name
  end

  def section_name
    @section_name
  end

  def funcs
    @funcs
  end

  def ctors
    @ctors
  end

  def parent
    @parent
  end

  def parent=(parent)
    @parent = parent
  end
end

class CheatsheetFunc
  def initialize(name)
    @name = name
    @type = "?"
    @args = Array.new
  end

  def name
    @name
  end

  def type
    @type
  end

  def type=(type)
    @type = type
  end

  def args
    @args
  end

  def args=(args)
    @args = args
  end
end

def create_cheatsheet()
  classes = Array.new
  Dir.glob("*/**/*.cpp").each do |file|
    if File.expand_path(file).include?("FSEChaiLib.cpp")
      classes += extract_from_base_chailib(File.read(File.expand_path(file)))
      next
    end
    contents = File.read(File.expand_path(file))
    if contents.include? "FSE_CHAI_REGISTER"
      name = contents.match(/FSE_CHAI_REGISTER\((.*)\)/)[1]
      section = File.expand_path(file).split("/")[-2]
      clz = CheatsheetClass.new(name, section)
      parent = contents.match(/chaiscript::base_class<(.*),/)
      clz.parent = parent[1] unless parent.nil?
      fill_funcs(clz, contents)
      fill_ctors(clz, contents)
      classes << clz
    end
  end

  classes.sort { |a, b| a.section_name <=> b.section_name }

  cheatsheet = create_base_sheet

  written_sections = Array.new

  classes.each do |clz|
    unless written_sections.include?(clz.section_name)
      cheatsheet += "#### Section: #{clz.section_name}\n\n"
      written_sections << clz.section_name
    end

    cheatsheet += generate_type_markdown(clz)
  end

  File.write("fse_chai_cheatsheet.md", cheatsheet)
end

def create_base_sheet
  cheatsheet =
    %q(# FSE CHAISCRIPT CHEATSHEET
    
This cheatsheet is FSEs addition to the regular Chaiscript cheatsheet 
    
## Basic Funs
    
- int **random**(int from, int to)
- float **random**(float from, float to)
- double **random**(double from, double to)
      
- **exit**() closes ingame console 
- **clear**() clears ingame console 
    
**puts** and **print** print to the ingame console
    
## TYPES

)

  return cheatsheet
end

def generate_type_markdown(clz)
  result = ""

  result += "#### #{clz.name}\n"

  unless clz.parent.nil?
    parentstring = "#{clz.parent}"
    parentstring.gsub!("fse::", "")
    parentstring.gsub!("std::", "")
    parentstring.gsub!("sf::", "")
    result += "\n##### Parent: [#{parentstring}](##{parentstring.downcase.gsub(" ", "-")})  \n"
  end

  unless clz.ctors.empty?
    result += "\n##### CTORS\n"
    clz.ctors.each do |ctor|
      ctor_arg_string = "#{ctor.args}"
      ctor_arg_string.gsub!("[", "")
      ctor_arg_string.gsub!("]", "")
      ctor_arg_string.gsub!('"', "")
      ctor_arg_string.gsub!("&", "")
      ctor_arg_string.gsub!("*", "")
      ctor_arg_string.gsub!("fse::", "")
      ctor_arg_string.gsub!("std::", "")
      ctor_arg_string.gsub!("sf::", "")
      ctor_arg_string.gsub!("const ", "")

      result += "- **#{ctor.type}**(#{ctor_arg_string})\n"
    end
  end

  unless clz.funcs.empty?
    result += "\n##### Funcs\n"
    clz.funcs.each do |func|
      func_arg_string = "#{func.args}"
      func_arg_string.gsub!("[", "")
      func_arg_string.gsub!("]", "")
      func_arg_string.gsub!('"', "")
      func_arg_string.gsub!("&", "")
      func_arg_string.gsub!("*", "")
      func_arg_string.gsub!("fse::", "")
      func_arg_string.gsub!("std::", "")
      func_arg_string.gsub!("sf::", "")
      func_arg_string.gsub!("const ", "")

      func_type_string = "#{func.type}"
      func_type_string.gsub!("&", "")
      func_type_string.gsub!("*", "")
      func_type_string.gsub!("fse::", "")
      func_type_string.gsub!("std::", "")
      func_type_string.gsub!("sf::", "")
      func_type_string.gsub!("const ", "")

      if (func_type_string.include? ":attr:")
        result += "- #{func_type_string} **#{func.name}**\n"
      else
        result += "- #{func_type_string} **#{func.name}**(#{func_arg_string})\n"
      end
    end
  end

  return result
end

def extract_from_base_chailib(original_file_contents)
  classes = Array.new

  startindex = original_file_contents.index("chai.add(chaiscript::user_type")
  while startindex != nil
    nextstartindex = original_file_contents.index("chai.add(chaiscript::user_type", startindex + 12)

    contents = nil
    if nextstartindex.nil?
      contents = original_file_contents[startindex..-1]
    else
      contents = original_file_contents[startindex..nextstartindex]
    end
    name = contents.match(/chaiscript::user_type<(.*)>/)[1]
    name.gsub!("fse::", "")
    name.gsub!("std::", "")
    name.gsub!("sf::", "")
    section = "BaseLib"
    clz = CheatsheetClass.new(name, section)
    parent = contents.match(/chaiscript::base_class<(.*),/)
    clz.parent = parent[1] unless parent.nil?
    fill_funcs(clz, contents)
    fill_ctors(clz, contents)
    classes << clz

    startindex = nextstartindex
  end
  return classes
end

def fill_ctors(cheatsheet_class, file_contents)
  startindex = file_contents.index("chaiscript::constructor")
  while startindex != nil
    nextstartindex = file_contents.index("chaiscript::constructor", startindex + 12)

    nameindex = file_contents.index('"', startindex)
    name = file_contents[nameindex + 1..file_contents.index('"', nameindex + 1) - 1].strip

    func = CheatsheetFunc.new(name)
    func.type = name

    argindex_search_string = "<#{name}("
    argindex = file_contents.index(argindex_search_string, startindex)

    if argindex.nil?
      argindex_search_string = "<#{name}>("
      argindex = file_contents.index(argindex_search_string, startindex)
    end

    if argindex.nil?
      argindex_search_string = "<sf::#{name}("
      argindex = file_contents.index(argindex_search_string, startindex)
    end

    if argindex.nil?
      argindex_search_string = "<fse::#{name}("
      argindex = file_contents.index(argindex_search_string, startindex)
    end

    if argindex.nil?
      argindex_search_string = "<std::#{name}("
      argindex = file_contents.index(argindex_search_string, startindex)
    end

    argsstring = file_contents[argindex + argindex_search_string.length..file_contents.index(")", argindex + argindex_search_string.length) - 1]

    if (!argsstring.nil? && argsstring != "")
      arg_array = argsstring.split(",")
      arg_array.each { |a| a.strip! }
      func.args = arg_array
    end

    cheatsheet_class.ctors << func

    startindex = nextstartindex
  end
end

def fill_funcs(cheatsheet_class, file_contents)
  startindex = file_contents.index("chaiscript::fun")
  while startindex != nil
    nextstartindex = file_contents.index("chaiscript::fun", startindex + 24)

    nameindex = file_contents.index('"', startindex)
    name = file_contents[nameindex + 1..file_contents.index('"', nameindex + 1) - 1].strip

    func = CheatsheetFunc.new(name)
    typeindex = nil
    typeindex = file_contents.index("static_cast", startindex)
    if (!typeindex.nil? && (nextstartindex.nil? || typeindex < nextstartindex))
      type = file_contents[typeindex + 12..file_contents.index("(", typeindex + 12) - 1].strip
      func.type = type

      attrindex = file_contents.index(")(", typeindex)
      unless attrindex.nil?
        attrstring = file_contents[attrindex + 2..file_contents.index(")", attrindex + 2) - 1]
        if (!attrstring.nil? && attrstring != "")
          arg_array = attrstring.split(",")
          if (arg_array[0].include?(cheatsheet_class.name))
            arg_array.delete_at(0)
          end
          arg_array.each { |a| a.strip! }
          func.args = arg_array
        end
      end
    else
      typeindex = file_contents.index("[](", startindex)
      if (!typeindex.nil? && (nextstartindex.nil? || typeindex < nextstartindex))
        func.type = "?"

        attrstring = file_contents[typeindex + 3..file_contents.index(")", typeindex + 3) - 1]
        if (!attrstring.nil? && attrstring != "")
          arg_array = attrstring.split(",")
          if (arg_array[0].include?(cheatsheet_class.name))
            arg_array.delete_at(0)
          end
          arg_array.each { |a| a.strip! }
          func.args = arg_array
        end
      else
        func.type = ":attr:"
      end
    end

    cheatsheet_class.funcs << func

    startindex = nextstartindex
  end
end

if __FILE__ == $PROGRAM_NAME
  main()
end
