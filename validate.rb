dir = File.expand_path File.dirname(__FILE__)
require "#{dir}/../common/validator/validator.rb"

validator = AXRValidator::Validator.new
scanner = AXRValidator::FileScanner.new

# Default is :fs
scanner.source = :cached if ARGV.include? "--cached"

scanner.on(/.*\.(h|c|cpp|m|mm)$/) do |file|
  generic = AXRValidator::Validators::Generic.new validator, file
  generic.validate_encoding_utf8
  generic.validate_whitespace
  generic.validate_indentation " "
end

validator.messages.each { |msg| puts "#{msg.to_string}\n\n" }
validator.end
