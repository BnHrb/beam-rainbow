class Reponse < ActiveRecord::Base
  belongs_to :user
  belongs_to :choix
end
