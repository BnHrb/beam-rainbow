class AddEntrepriseIdToSondages < ActiveRecord::Migration
  def change
    add_column :sondages, :entreprise_id, :integer
  end
end
