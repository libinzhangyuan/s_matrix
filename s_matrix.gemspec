# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 's_matrix/version'

Gem::Specification.new do |spec|
  spec.name          = "s_matrix"
  spec.version       = SMatrix::VERSION
  spec.authors       = ["zhangyuan"]
  spec.email         = ["libinzhangyuan@gmail.com"]
  spec.summary       = %q{educe memory usage for a large number of config. for example game config in excel.}
  spec.description   = %q{not finish}
  spec.homepage      = "https://github.com/libinzhangyuan/s_matrix"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.6"
  spec.add_development_dependency "rake", "~> 0"
  spec.add_development_dependency "rspec"
end
