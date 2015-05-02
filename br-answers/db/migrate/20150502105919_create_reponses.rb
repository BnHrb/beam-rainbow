class CreateReponses < ActiveRecord::Migration
  def change
    create_table :reponses do |t|
      t.string :valeur

      t.timestamps null: false
    end
  end
end
