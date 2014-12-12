Current version 0.0.4

### Matrix reduce memory usage for large number of config that do not change after initializing.
  Matrix save the hash like this: <br>
  # {<br>
  #  '234234' => {'id' => '23232', 'name' => 'haha'},<br>
  #  '234235' => {'id' => '23235', 'name' => 'haha2', 'hp' => '232'}<br>
  # }<br>
  For example, loading ./spec/equipment\_strengthen.plist will use 480M when using ruby Hash like this:<br>
  #      {'234232' => {id: '23423', name: 'haha'}}<br>
   Because there are about 1440000 ruby String objects when loading to memory.<br>
     And it will use 5M when using SMatrix.<br>
Result from test:   rspec ./spec/performance\_spec.rb<br>

## Installation

Add this line to your application's Gemfile:

    gem 's_matrix'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install s_matrix


## Usage
### initialize the config const
  # every thing will change to string (.to\_s) when saving to SMatrix<br>
  EQUIPMENTS = SMatrix.new
  EQUIPMENTS.add\_row('10001', {'id' => '10001', 'name' => 'wood sword', 'attack' => '342'})
  EQUIPMENTS.add\_row('20002', {id: 10002, name: 'shoe', speed: 5})

### use the config const
  # everything get back will be string<br>
  EQUIPMENTS\['10001'].should == {''id' => '10001', 'name' => 'wood sword', 'attack' => '342''}<br>
  EQUIPMENTS\['10001']['id'].should == '10001'<br>
  EQUIPMENTS\['10001'][:id].should == '10001'<br>
  EQUIPMENTS\['20002'].should == {'id' => '10002', 'name' => 'shoe', 'speed' => '5'}<br>
  EQUIPMENTS\['20002']['attack'].should == '552'<br>
  # or you can use get\_row instead of []<br>
  EQUIPMENTS.get\_row('20002')<br>
