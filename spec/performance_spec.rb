require 'spec_helper'
require 'xml_load'

describe SMatrix do
  describe 'test id' do
    it '' do
      #获得当前执行文件的完整路径
      plist_path = Pathname.new(File.dirname(__FILE__)).realpath.to_s

      GC.start
      p 'before load to const'
      puts `ps aux | grep ruby | grep spec`
      p ''
      Const.load_to_const_with_keys('EQUIPMENT_STRENGTHEN', [:equipment_id, :level], plist_path + '/equipment_strengthen.plist')
      GC.start
      p 'after load to const'
      puts `ps aux | grep ruby | grep spec`
      p ''


      a = SMatrix.new
      Const::EQUIPMENT_STRENGTHEN.each do |equip_id, hash_with_lev|
        hash_with_lev.each do |lev, info|
          a.add_row("#{equip_id}:#{lev}", info)
        end
      end

      GC.start
      p 'after load_to_s_matix after GC'
      puts `ps aux | grep ruby | grep spec`
      p ''


      EQUIP = a
      GC.start
      p 'after set a const to smatix'
      puts `ps aux | grep ruby | grep spec`
      p ''

      p 'size of smatix'
      puts a.size

      #puts EQUIP.to_s
    end

  end
end

