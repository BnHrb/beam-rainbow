class CreateSondages < ActiveRecord::Migration
  def change
    create_table :sondages do |t|
      t.string :nom
      t.date :date_publication

      t.timestamps null: false
    end
  end
end
