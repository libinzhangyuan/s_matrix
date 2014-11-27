#require ‘pathname’
 

module Const
  def self.load_to_const_with_keys(const_string, keys, file)
      content_array = Plist.parse_xml(file)
      const_hash = {}

      case keys.size
      when 1
        content_array.each {|content|
          key = str_to_key(content[keys[0].to_s])
          const_hash[key] = data_type_conversion(content)
        }

      when 2
        content_array.each {|content|
          key0 = str_to_key(content[keys[0].to_s])
          key1 = str_to_key(content[keys[1].to_s])

          const_hash[key0] = {} if const_hash[key0].nil?
          const_hash[key0][key1] = data_type_conversion(content)
        }

      else
        raise 'not support more than 3 indexes currently'
      end
      result = const_hash
      Const.const_set(const_string, result)
  end

  def self.str_to_key(str)
    is_number?(str) ? str.to_i : str.strip.to_sym
  end

  def self.is_number?(str)
    str.to_i.to_s == str
  end

  def self.data_type_conversion(hash)
    tmp_result = {}
    hash.each do |k, v|
      tmp_result[k.to_sym] = v
      next unless v.kind_of?(String)
      tmp_result[k.to_sym] = v.strip

      i_number = v.to_i
      str = v.strip
      next tmp_result[k.to_sym] = i_number if i_number.to_s == str
      f_number = v.to_f
      next tmp_result[k.to_sym] = f_number if f_number.to_s == str
    end
    tmp_result
  end
end
