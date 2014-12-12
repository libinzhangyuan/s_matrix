# SMatrix
Please coding following the 'Code Compelte' book.
And please write the rspec test.

## Contributing
1. Fork it ( https://github.com/libinzhangyuan/s_matrix/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request

##  Compile
rake clean
rake compile

## Test
rspec ./spec

# how to build gem
gem build s_matrix.gemspec
gem install ./s_matrix-0.0.1.gem  # please change version number to current one. Find version at lib/s_matrix/version.rb.
gem push s_matrix-0.0.1.gem
