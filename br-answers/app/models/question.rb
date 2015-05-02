class Question < ActiveRecord::Base
  belongs_to :sondage
  has_many :choixes
end
