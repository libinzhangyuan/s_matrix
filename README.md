Current version 1.0.0
Latest stable version is 1.0.0

### Matrix reduce memory usage for large Hash.
  Matrix store the hash like this: <br>
  # {<br>
  #  '234234' => {'id' => '23232', 'name' => 'haha'},<br>
  #  '234235' => {'id' => '23235', 'name' => 'haha2', 'hp' => '232'}<br>
  #  '234236' => {'id' => '23235', 'name' => 'haha2', 'attack' => '2322'}<br>
  # }<br>
  For example, loading a game config ./spec/equipment\_strengthen.plist will use 480M when using ruby Hash like this:<br>
  #      {'234232' => {id: '23423', name: 'haha'}}<br>
   Because there are about 1440000 ruby String objects when loading to memory.<br>
     And it will use 5M when using SMatrix.<br>
Result from test:   rspec ./spec/performance\_spec.rb<br>

## Installation

Add this line to your application's Gemfile:

    gem 's_matrix', '~>1.0'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install s_matrix


## Usage
### initialize the config const
  # every thing will change to string (.to\_s) when saving to SMatrix<br>
  EQUIPMENTS = SMatrix.new<br>
  EQUIPMENTS.add\_row('10001', {'id' => '10001', 'name' => 'wood sword', 'attack' => '342'})<br>
  EQUIPMENTS.add\_row('20002', {id: 10002, name: 'shoe', speed: 5})<br>

### use the config const
  # everything get back will be string<br>
    EQUIPMENTS\['10001'].should == {'id' => '10001', 'name' => 'wood sword', 'attack' => '342''}<br>
    EQUIPMENTS\['10001']['id'].should == '10001'<br>
    EQUIPMENTS\['10001'][:id].should == '10001'<br>
    EQUIPMENTS\['20002'].should == {'id' => '10002', 'name' => 'shoe', 'speed' => '5'}<br>
    EQUIPMENTS\['20002']['attack'].should == '552'<br>
    EQUIPMENTS.size.should == 2
  # or you can use get\_row instead of []<br>
    EQUIPMENTS.get\_row('20002')<br>
  # each
    EQUIPMENTS.each {|k, v| k == '10001', v == {'id' => '10001', 'name' => 'sword', ...} }
  # all
    EQUIPMENTS.all.should == { '20002' => {'id' => '10002', 'name' => 'shoe', 'speed' => '5'}, '20003' => {} }
  # ids
    EQUIPMENTS.ids.should == ['20002', '20003']
  # keys
    EQUIPMENTS.keys.should == ['id', 'name', 'hp']
  # to\_s for debug
    puts EQUIPMENTS.to_s

### todolist
  https://github.com/libinzhangyuan/s\_matrix/blob/master/todolist.txt
