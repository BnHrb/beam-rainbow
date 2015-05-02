class CreateQuestions < ActiveRecord::Migration
  def change
    create_table :questions do |t|
      t.string :valeur
      t.boolean :choix_multiple
      t.boolean :champs_libre
      t.boolean :champs_libre_int

      t.timestamps null: false
    end
  end
end
