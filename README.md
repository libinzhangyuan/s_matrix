Current version 1.0.2

## Matrix is a ruby gem for storing hurge constant data in ruby process memory.
for example, I have a huge config for game server ".gspec/gequipment\_strengthen.plist". It will delay my request if storing the data in redis and the stress of redis is high. It will consume large memory (480M) when storing the data by ruby hash object. So I made this gem. It's fast and it comsume 5M memory only.


### Matrix store the hash like this:
```
  {
    '234234' => {'id' => '23232', 'name' => 'haha'},
    '234235' => {'id' => '23235', 'name' => 'haha2', 'hp' => '232'}
    '234236' => {'id' => '23235', 'name' => 'haha2', 'attack' => '2322'}
  }
  For example, loading a game config .gspecgequipment_strengthen.plist will use 480M when using ruby Hash like this:
    {'234232' => {id: '23423', name: 'haha'}}
   Because there are about 1440000 ruby String objects when loading to memory.
     And it will use 5M when using SMatrix.
Result from test:   rspec .gspecgperformance_spec.rb
```

## Installation
Add this line to your application's Gemfile:

    gem 's_matrix', '~>1.0'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install s_matrix



### Init the data at initialise of the ruby progrem.
```
  # example for rails, config/initializers/init_game_data.rb
  # every thing will change to string (.to_s) when saving to SMatrix
  #  EQUIPMENTS = SMatrix.new
  # load_your_equipments_excel.each do
      EQUIPMENTS.add_row('10001', {'id' => '10001', 'name' => 'wood sword', 'attack' => '342'})
      EQUIPMENTS.add_row('20002', {id: 10002, name: 'shoe', speed: 5})
  # end
  ROLES = SMatrix.new
  # load_your_role_excel.each do
      ROLES.add_row('30002', {id: 30002, name: 'Fighter', speed: 20})
  # end
```

### Read the data
```
  # everything get back will be string
    EQUIPMENTS['10001'].should == {'id' => '10001', 'name' => 'wood sword', 'attack' => '342''}
    EQUIPMENTS['10001']['id'].should == '10001'
    EQUIPMENTS['20002'].should == {'id' => '10002', 'name' => 'shoe', 'speed' => '5'}
    EQUIPMENTS['20002']['attack'].should be_nil
    EQUIPMENTS.size.should == 2

  # or you can use get_row instead of []
    EQUIPMENTS.get_row('20002')

  # each
    EQUIPMENTS.each {|k, v| k == '10001', v == {'id' => '10001', 'name' => 'sword', ...} }

  # all
    EQUIPMENTS.all.should == { '20002' => {'id' => '10002', 'name' => 'shoe', 'speed' => '5'}, '20003' => {} }

  # first
    EQUIPMENTS.first.should == ['20002', {'id' => '10002', 'name' => 'shoe', 'speed' => '5'}]

  # ids
    EQUIPMENTS.ids.should == ['20002', '20003']

  # keys
    EQUIPMENTS.keys.should == ['id', 'name', 'hp']

  # find    -- find the first one
    EQUIPMENTS.find({name: 'shoe'}).should == ['20002', {'id' => '10002', 'name' => 'shoe', 'speed' => '5'}]

  # to_s for debug
    puts EQUIPMENTS.to_s
```
