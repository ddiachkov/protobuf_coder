# encoding: utf-8
Gem::Specification.new do |gem|
  gem.authors        = "Denis Diachkov"
  gem.email          = "d.diachkov@gmail.com"
  gem.description    = %q{TODO: Write a gem description}
  gem.summary        = %q{TODO: Write a gem summary}
  gem.homepage       = "https://github.com/ddiachkov/protobuf_coder"

  gem.files          = `git ls-files`.split( "\n" )
  gem.test_files     = `git ls-files -- {test,spec,features}/*`.split( "\n" )
  gem.name           = "protobuf_coder"
  gem.extensions    << "ext/extconf.rb"
  gem.require_paths  = "ext"
  gem.version        = "0.0.1"
end