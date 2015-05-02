class Sondage < ActiveRecord::Base
  has_many :questions
  belong_to :entreprise
  has_and_belongs_to_many :users
end
