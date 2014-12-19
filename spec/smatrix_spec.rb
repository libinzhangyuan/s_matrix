require 'spec_helper'

describe SMatrix do
  describe 'test id' do
    it '' do
      a = SMatrix.new
      expect(a.add_row('2', {'a' => '2'})).to eq(a)
      expect(a.add_row(2, {a: 2})).to eq(a)
      expect(a.add_row(:symbel_is_ok, {a: 2})).to eq(a)

      expect {a.add_row('', {a: 2})}.to raise_error
      expect {a.add_row(nil, {a: 2})}.to raise_error
    end
  end

  describe 'test row' do
    it '' do
      a = SMatrix.new
      expect {a.add_row('222', 1)}.to raise_error
      expect {a.add_row('222', [1, 2])}.to raise_error
    end
  end

  describe 'test to_s' do
    it '' do
      a = SMatrix.new
      puts a.to_s
      a.add_row(2, {a: 2})
      puts a.to_s
    end
  end

  describe 'test add and get' do
    it '' do
      a = SMatrix.new
      expect(a.add_row('2', {a: 2})).to eq(a)
      rst = a.get_row('2')
      expect(a.get_row('2')).to eq({'a' => '2'})
    end

    it '' do
      a = SMatrix.new
      a.add_row('2', {a: 2})
      a.add_row('3', {b: 3})

      expect(a.get_row('2')).to eq({'a' => '2', 'b' => nil})
      expect(a.get_row('3')).to eq({'a' => nil, 'b' => '3'})

      a.add_row('4', {a: 4, c: 5})
      expect(a.get_row('2')).to eq({'a' => '2', 'b' => nil, 'c' => nil})
      expect(a.get_row('3')).to eq({'a' => nil, 'b' => '3', 'c' => nil})
      expect(a.get_row('4')).to eq({'a' => '4', 'b' => nil, 'c' => '5'})

    end

    it '' do
      a = SMatrix.new
      a.add_row('2', {a: 2})
      a.add_row('2', {b: 3})
      expect(a.get_row('2')).to eq({'a' => nil, 'b' => '3'})
    end
  end

  describe 'each' do
    it '' do
      a = SMatrix.new
      a.add_row('2', {a: 2})
      a.add_row('3', {b: 3})

      # must 2 params with block
      #expect { a.each }.to raise_error
      #expect { a.each {|a|} }.to raise_error
      #expect { a.each {|a, b, c|} }.to raise_error


      # key and value is right
      hash = {}
      a.each do |k, v|
        hash[k] = v
      end
      expect(hash).to eq({
        '2' => {'a' => '2', 'b' => nil},
        '3' => {'a' => nil, 'b' => '3'}
      })
    end
  end

  describe '[]' do
    it '' do
      a = SMatrix.new
      a.add_row('2', {a: 2})
      a.add_row('2', {b: 3})
      expect(a['2']).to eq({'a' => nil, 'b' => '3'})
      expect(a['2'][:a]).to eq nil
      expect(a['2']['b']).to eq '3'
    end
  end

  describe 'all' do
    it '' do
      a = SMatrix.new
      expect(a.all).to eq({})
      a.add_row('2', {a: 2})
      expect(a.all).to eq({'2' => {'a' => '2'}})
      a.add_row('2', {b: 3})
      expect(a.all).to eq({'2' => {'a' => nil, 'b' => '3'}})
      a.add_row('3', {b: 3})
      expect(a.all).to eq({
        '2' => {'a' => nil, 'b' => '3'},
        '3' => {'a' => nil, 'b' => '3'}
        })
    end
  end

end

