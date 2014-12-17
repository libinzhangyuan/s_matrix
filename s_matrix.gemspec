# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 's_matrix/version'

Gem::Specification.new do |spec|
  spec.name          = "s_matrix"
  spec.version       = SMatrix::VERSION
  spec.authors       = ["zhangyuan"]
  spec.email         = ["libinzhangyuan@gmail.com"]
  spec.summary       = %q{Reduce memory usage for a large Hash.}
  spec.description   = %q{Matrix store the hash like this: {'234234' => {'id' => '23232', 'name' => 'haha'}, ...}}
  spec.homepage      = "https://github.com/libinzhangyuan/s_matrix"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]
  spec.extensions = %w[ext/s_matrix/extconf.rb]

  spec.add_development_dependency "bundler", "~> 1.6"
  spec.add_development_dependency "rake", "~> 10.1"


  spec.add_development_dependency "rspec", "~> 3.1"
  spec.add_development_dependency "rake-compiler", "~> 0.9"


  # for testing
  spec.add_development_dependency "plist", "~> 3.1"
end
