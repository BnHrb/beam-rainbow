class CreateUsers < ActiveRecord::Migration
  def change
    create_table :users do |t|
      t.date :naissance
      t.string :sexe
      t.string :etat_civil
      t.integer :pop_foyer
      t.integer :pop_enfant
      t.integer :pop_animaux
      t.string :revenus

      t.timestamps null: false
    end
  end
end
