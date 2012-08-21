#!/usr/bin/env rake
require "bundler/gem_tasks"
require "rake/clean"

SRCDIR  = "ext"
EXTNAME = "protobuf_coder.#{Config::MAKEFILE_CONFIG[ "DLEXT" ]}"

CLEAN.include "#{SRCDIR}/*.o", "#{SRCDIR}/#{EXTNAME}", "#{SRCDIR}/Makefile", "#{SRCDIR}/mkmf.log"

file "#{SRCDIR}/Makefile" do
  chdir( SRCDIR ) { ruby "extconf.rb" }
end

file EXTNAME => FileList[ "#{SRCDIR}/Makefile" ] do
  chdir( SRCDIR ) { sh "make" }
end

task :build => EXTNAME
task :default => [ :clean, :build ]

desc "Compile extension"
task :compile => [ :clean, EXTNAME ]