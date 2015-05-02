class Choix < ActiveRecord::Base
  belongs_to :question
  has_many :users, :through => :reponses
end
