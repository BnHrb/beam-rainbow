class CreateChoixes < ActiveRecord::Migration
  def change
    create_table :choixes do |t|
      t.string :valeur

      t.timestamps null: false
    end
  end
end
