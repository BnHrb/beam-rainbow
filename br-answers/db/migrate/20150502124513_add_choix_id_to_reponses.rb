class AddChoixIdToReponses < ActiveRecord::Migration
  def change
    add_column :reponses, :choix_id, :integer
  end
end
