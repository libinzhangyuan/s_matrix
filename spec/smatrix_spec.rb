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

  describe 'all && ids && keys' do
    it '' do
      a = SMatrix.new
      expect(a.all).to eq({})
      expect(a.ids).to eq([])
      expect(a.keys).to eq([])
      expect(a.first).to eq(nil)

      a.add_row('2', {a: 2})
      expect(a.all).to eq({'2' => {'a' => '2'}})
      expect(a.ids).to eq(['2'])
      expect(a.keys).to eq(['a'])
      expect(a.first).to eq(['2', {'a' => '2'}])

      a.add_row('2', {b: 3})
      expect(a.all).to eq({'2' => {'a' => nil, 'b' => '3'}})
      expect(a.ids).to eq(['2'])
      expect(a.keys).to eq(['a', 'b'])
      expect(a.first).to eq(['2', {'a' => nil, 'b' => '3'}])

      a.add_row('3', {b: 3})
      expect(a.all).to eq({
        '2' => {'a' => nil, 'b' => '3'},
        '3' => {'a' => nil, 'b' => '3'}
        })
      expect(a.ids).to eq(['2', '3'])
      expect(a.keys).to eq(['a', 'b'])
      expect(a.first == ['2', {'a' => nil, 'b' => '3'}] || a.first == ['3', {'b' => '3'}]).to eq(true)
    end
  end

  describe 'find' do
    it '' do
      a = SMatrix.new
      expect( a.find({a: '2'}) ).to eq nil

      a.add_row('223', {a: 2})
      expect( a.find({a: '2'}) ).to eq ['223', {'a' => '2'}]

      a.add_row('224', {a: 3})
      expect( a.find({a: '2'}) ).to eq ['223', {'a' => '2'}]
      expect( a.find({a: '3'}) ).to eq ['224', {'a' => '3'}]
      expect( a.find({a: '3', b: nil}) ).to eq ['224', {'a' => '3'}]

      a.add_row('225', {a: 2, b: 3})
      expect( a.find({a: '2'})[0] == '223' || a.find({a: '2'})[0] == '224' ).to eq true
      expect( a.find({b: '3'}) ).to eq ['225', {'a' => '2', 'b' => '3'}]
      expect( a.find({a: '2', b: '3'}) ).to eq ['225', {'a' => '2', 'b' => '3'}]
      expect( a.find({a: '2', b: '1'}) ).to eq nil
      expect( a.find({a: nil, b: '3'}) ).to eq nil
    end
  end

  describe 'find_all' do
    it '' do
      a = SMatrix.new
      expect( a.find_all({a: '2'}) ).to eq []

      a.add_row('223', {a: 2})
      expect( a.find_all({a: '2'}) ).to eq [['223', {'a' => '2'}]]

      a.add_row('225', {a: 2, b: 3})
      expect( a.find_all({a: '2'}).size ).to eq 2
      expect( (a.find_all({a: '2'})[0][0] == '223' && a.find_all({a: '2'})[1][0] == '225') ||
              (a.find_all({a: '2'})[1][0] == '223' && a.find_all({a: '2'})[0][0] == '225') ).to eq true

      expect( a.find_all({b: '3'})).to eq [['225', {'a' => '2', 'b' => '3'}]]
      expect( a.find_all({a: '2', b: '3'})).to eq [['225', {'a' => '2', 'b' => '3'}]]
      expect( a.find_all({a: '2', b: '4'}) ).to eq []
      expect( a.find_all({a: nil, b: '3'})).to eq []

      a.add_row('226', {b: 4})
      expect( a.find_all({b: '4'})).to eq [['226', {'a' => nil, 'b' => '4'}]]
      expect( a.find_all({a: nil, b: '4'})).to eq [['226', {'a' => nil, 'b' => '4'}]]
    end
  end


end

